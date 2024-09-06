
#include "png_exporter.hpp"

#include "utils.hpp"
#include "logger.hpp"

#include <cstring>
#include <fstream>
#include <filesystem>


using namespace std;
namespace fs = std::filesystem;


const uint8_t PngExporter::PNG_HEADER[] = {137, 80, 78, 71, 13, 10, 26, 10};

const uint8_t PngExporter::IEND_CHUNK[] = {
    0, 0, 0, 0,
    'I', 'E', 'N', 'D',
    0xAE, 0x42, 0x60, 0x82
};

const uint8_t PngExporter::IHDR_CHUNK_TYPE[]= {
    'I', 'H', 'D', 'R'
};

const uint8_t PngExporter::IDAT_CHUNK_TYPE[]= {
    'I', 'D', 'A', 'T'
};

bool PngExporter::crc_table_computed = false;
uint32_t PngExporter::crc_table[256] = {0};

const uint16_t PngExporter::DEFLATE_BLOCK_MAX_SIZE = 0xffffu;
const uint16_t PngExporter::NOT_DEFLATE_BLOCK_MAX_SIZE = 0x0000u;


void PngExporter::WriteIHDRChunk()
{
    uint32_t total_IHDR_len = 25;
    auto chunk_buffer = make_unique<uint8_t[]>(total_IHDR_len);
    uint8_t *dest = chunk_buffer.get();

    change_endianess((uint32_t) 13, dest);

    mempcpy(dest + 4, PngExporter::IHDR_CHUNK_TYPE, 4);
    
    change_endianess(m_width, dest + 8);
    change_endianess(m_height, dest + 12);
    // bit depth : each color is a 8 bit value
    dest[16] = 8;
    // color type : 2 for RGB triplet
    dest[17]= 2;
    // compression method : 0 for default
    dest[18] = 0;
    // filter method : 0 for default
    dest[19] = 0;
    // interlace method : 0 for none
    dest[20] = 0;

    uint32_t crc = compute_crc(dest + 4, 13 + 4);
    change_endianess(crc, dest + 21);

    m_output_fs.write((char*) dest, total_IHDR_len);
}


void PngExporter::deflate_compress(uint8_t *src, size_t len, uint8_t *dest)
{
    uint32_t times = len / DEFLATE_BLOCK_MAX_SIZE;
    uint16_t remainder = len % DEFLATE_BLOCK_MAX_SIZE;
    uint16_t not_remainder = ~remainder;

    uint8_t *data_ptr = dest;
    for(uint32_t i=0; i<times; ++i){
        // info : 5 options | 2 compression type | 1 last block flag
        data_ptr[0] = 0x00;
        memcpy(data_ptr + 1, &DEFLATE_BLOCK_MAX_SIZE, 2);
        memcpy(data_ptr + 3, &NOT_DEFLATE_BLOCK_MAX_SIZE, 2);

        memcpy(data_ptr + 5, src + i*DEFLATE_BLOCK_MAX_SIZE, DEFLATE_BLOCK_MAX_SIZE);
        data_ptr += 5 + DEFLATE_BLOCK_MAX_SIZE;
    }

    // last block
    data_ptr[0] = 0x01;
    memcpy(data_ptr + 1, &remainder, 2);
    memcpy(data_ptr + 3, &not_remainder, 2);

    memcpy(data_ptr + 5, src + times*DEFLATE_BLOCK_MAX_SIZE, remainder);
}


void PngExporter::ImageToScanline(uint8_t filtering, uint8_t *dest) const
{
    uint8_t *data_ptr = dest;
    for(uint32_t j=0; j<m_height; ++j){
        *data_ptr++ = filtering;
        for(uint32_t i=0; i<m_width; ++i){
            RGBColor color = m_data_buffer[j*m_width + i];
            color[0] = linear_to_gamma(color[0]);
            color[1] = linear_to_gamma(color[1]);
            color[2] = linear_to_gamma(color[2]);
            rgb_normalized_to_8bits(color, data_ptr);
            data_ptr += 3;
        }
    }
}


void PngExporter::WriteIDATChunk()
{
    uint32_t scanline_image_size = 3*m_width*m_height + m_height;
    auto scanline_image = make_unique<uint8_t[]>(scanline_image_size);
    
    uint8_t filtering = 0x00;
    ImageToScanline(filtering, scanline_image.get());

    uint32_t deflate_blocks_overhead = ((scanline_image_size / DEFLATE_BLOCK_MAX_SIZE) + 1) * 5;
    uint32_t zlib_overhead = 1 + 1 + 4;
    uint32_t idat_chunk_overhead = 4 + 4 + 4;                                                           
    auto idat_chunk = make_unique<uint8_t[]>(
        scanline_image_size + deflate_blocks_overhead + zlib_overhead + idat_chunk_overhead
    );

    uint8_t *dest = idat_chunk.get();
    change_endianess(scanline_image_size + deflate_blocks_overhead + zlib_overhead, dest);
    memcpy(dest+4, PngExporter::IDAT_CHUNK_TYPE, 4);

    // CMF : 4 compression window | 4 compression method (8 on PNG)
    uint8_t cmf = 0b00001000;
    // FLG : 2 compression level | 1 dictionary | 5 completion
    // the 5 completion bits must make (CMF | FLG) = 0 mod 31
    uint8_t flg = 0b00011101;
    dest[8] = cmf;
    dest[9] = flg;
    
    deflate_compress(scanline_image.get(), scanline_image_size, dest + 10);

    uint32_t adler = adler32(scanline_image.get(), scanline_image_size);
    change_endianess(adler, dest + 10 + scanline_image_size + deflate_blocks_overhead);

    uint32_t crc = compute_crc(dest + 4, 4 + zlib_overhead + deflate_blocks_overhead + scanline_image_size);
    change_endianess(crc, dest + 4 + 4 + zlib_overhead + deflate_blocks_overhead + scanline_image_size);

    m_output_fs.write((char*) dest, idat_chunk_overhead + zlib_overhead + deflate_blocks_overhead + scanline_image_size);
}


/* Make the table for a fast CRC. */
void PngExporter::compute_crc_table()
{
    uint32_t c;

    for (int n=0; n < 256; n++) {
        c = (uint32_t) n;
        for (int k=0; k < 8; k++) {
            if (c & 1) {
                c = 0xedb88320L ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }
        crc_table[n] = c;
    }
    
    PngExporter::crc_table_computed = 1;
}


/* Update a running CRC with the bytes buf[0..len-1]--the CRC
    should be initialized to all 1's, and the transmitted value
    is the 1's complement of the final running CRC (see the
    CRC() routine below)). */

uint32_t PngExporter::update_crc(uint32_t crc, uint8_t *buf, int len)
{
    uint32_t c = crc;

    if(!PngExporter::crc_table_computed){
        PngExporter::compute_crc_table();
    }
    for(int i=0; i < len; ++i) {
        c = crc_table[(c ^ buf[i]) & 0xff] ^ (c >> 8);
    }
    return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
uint32_t PngExporter::compute_crc(uint8_t *buf, size_t len)
{
    return PngExporter::update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}


// This reference implementation was taken from Wikipedia
uint32_t PngExporter::adler32(uint8_t *data, size_t len) 
{
    uint32_t a = 1, b = 0;
    
    // Process each byte of the data in order
    for (size_t i = 0; i < len; ++i)
    {
        a = (a + data[i]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    
    return (b << 16) | a;
}


int PngExporter::Export(int width, int height, shared_ptr<RGBColor[]> buffer)
{
    fs::path path(m_filepath);
    fs::path parentDir = path.parent_path();
    if( !fs::exists(parentDir) ){
        fs::create_directory(parentDir);
    }

    m_output_fs = ofstream(m_filepath, std::ios::out | std::ios::binary);
    if ( !m_output_fs.is_open()) {
        Logger::LogError(string(__FUNCTION__) + " : failed to open " + m_filepath);
        return 1;
    }
    
    m_width = width;
    m_height = height;
    m_data_buffer = buffer;

    m_output_fs.write((char*) PngExporter::PNG_HEADER, sizeof(PNG_HEADER));
    
    WriteIHDRChunk();
    WriteIDATChunk();

    m_output_fs.write((char*) PngExporter::IEND_CHUNK, sizeof(IEND_CHUNK));

    m_output_fs.close();

    return 0;
}