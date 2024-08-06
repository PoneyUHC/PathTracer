
#include "png_exporter.hpp"

#include "utils.hpp"

#include <cstring>
#include <fstream>

#include <zlib.h>

using namespace std;


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


void PngExporter::write_IHDR_chunk()
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


void PngExporter::write_IDAT_chunks(shared_ptr<RGBColor[]> buffer)
{
    uint32_t compressed_data_len = m_width*m_height*3 + m_height;
    uint32_t zlib_size = compressed_data_len + 11;
    auto zlib = make_unique<uint8_t[]>(zlib_size);
    uint8_t *dest = zlib.get();

    // CMF : 4 compression window | 4 compression method (8 on PNG)
    uint8_t cmf = 0b00001000;

    // FLG : 2 compression level | 1 dictionary | 5 completion
    // the 5 completion bits must make (CMF | FLG) = 0 mod 31
    uint8_t flg = 0b00011101;

    // info : 5 options | 2 compression type | 1 last block flag
    // only one block so last block flag on 
    uint8_t info = 0b00000001;


    // R, G, and B bytes + filtering tag
    uint16_t compressed_data_len = 3*m_width + 1;
    uint16_t not_compressed_data_len = ~compressed_data_len;

    uint8_t filtering = 0x00;

    dest[0] = cmf;
    dest[1] = flg;
    dest[2] = info;

    memcpy(dest + 2, &compressed_data_len, 2);
    memcpy(dest + 4, &not_compressed_data_len, 2);

    for(uint32_t j=0; j<m_height; ++j) {
        *dest = filtering;
        ++dest;
        for(uint32_t i=0; i<m_width; ++i){
            IExporter::double_to_byte(buffer[j*m_width + i], dest);
            dest += 3;
        }
    }

    uint32_t adler = adler32(dest + 15, compressed_data_len);
    change_endianess(adler, dest + 15 + compressed_data_len);


    

    // cmf + flg + info + len + nlen + adler
    // 1 + 1 + 1 + 2 + 2 + 4
    // + compressed data len
    uint32_t data_field_len = 11 + compressed_data_len;


    change_endianess(7, dest);
    memcpy(dest + 4, PngExporter::IDAT_CHUNK_TYPE, 4);
    
    dest[8] = cmf;
    dest[9] = flg;

    dest[10] = info;
    memcpy(dest + 11, &compressed_data_len, 2);
    memcpy(dest + 13, &not_compressed_data_len, 2);

    uint32_t crc = compute_crc(dest + 4, 7);
    change_endianess(crc, dest + 7);

    m_output_fs.write((char*) dest, 19);
    

    for(uint32_t j=0; j<m_height; ++j) {

        change_endianess(m_width*3+1, dest);
        memcpy(dest + 4, PngExporter::IDAT_CHUNK_TYPE, 4);

        uint8_t *pos = dest + 15;
        pos[0] = filtering;
        ++pos;
        for(uint32_t i=0; i<m_width; ++i){
            IExporter::double_to_byte(buffer[j*m_width + i], pos);
            pos += 3;
        }

        uint32_t crc = compute_crc(dest + 4, compressed_data_len);
        change_endianess(crc, dest + compressed_data_len);

        m_output_fs.write((char*) dest, 8 + compressed_data_len + 4);
    }

    change_endianess(7, dest);
    memcpy(dest + 4, PngExporter::IDAT_CHUNK_TYPE, 4);

    
}


/* Make the table for a fast CRC. */
void PngExporter::compute_crc_table()
{
    uint32_t c;
    int n, k;

    for (n = 0; n < 256; n++) {
        c = (uint32_t) n;
        for (k = 0; k < 8; k++) {
            if (c & 1) {
                c = 0xedb88320L ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }
        crc_table[n] = c;
    }
    crc_table_computed = 1;
}


/* Update a running CRC with the bytes buf[0..len-1]--the CRC
    should be initialized to all 1's, and the transmitted value
    is the 1's complement of the final running CRC (see the
    CRC() routine below)). */

uint32_t PngExporter::update_crc(uint32_t crc, uint8_t *buf, int len)
{
    uint32_t c = crc;

    if (!crc_table_computed){
        compute_crc_table();
    }
    for (int i = 0; i < len; ++i) {
        c = crc_table[(c ^ buf[i]) & 0xff] ^ (c >> 8);
    }
    return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
uint32_t PngExporter::compute_crc(uint8_t *buf, size_t len)
{
    return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
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
    m_output_fs = ofstream(m_filepath, std::ios::out | std::ios::binary);
    m_width = width;
    m_height = height;

    m_output_fs.write((char*) PngExporter::PNG_HEADER, sizeof(PNG_HEADER));
    
    write_IHDR_chunk();
    write_IDAT_chunks(buffer);

    m_output_fs.write((char*) PngExporter::IEND_CHUNK, sizeof(IEND_CHUNK));

    m_output_fs.close();

    return 0;
}