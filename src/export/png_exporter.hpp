
#pragma once

#include "export/IExporter.hpp"

#include <string>
#include <fstream>


class PngExporter: public IExporter {

private:

    uint32_t m_width;
    uint32_t m_height;
    std::shared_ptr<RGBColor[]> m_data_buffer;
    std::ofstream m_output_fs;

    std::string m_filepath;

    static const uint8_t PNG_HEADER[];
    static const uint8_t IEND_CHUNK[];
    static const uint8_t IHDR_CHUNK_TYPE[];
    static const uint8_t IDAT_CHUNK_TYPE[];

    static const uint16_t DEFLATE_BLOCK_MAX_SIZE;
    static const uint16_t NOT_DEFLATE_BLOCK_MAX_SIZE;

    static void deflate_compress(uint8_t *src, size_t len, uint8_t *dest);

    void ImageToScanline(uint8_t filtering, uint8_t *dest) const;
    void WriteIHDRChunk();


    // The DATA field from the IDAT chunk is the result of a zlib compression

    // ZLIB compression
    // +---+---+=====================+---+---+---+---+
    // |CMF|FLG|...compressed data...|    ADLER32    |
    // +---+---+=====================+---+---+---+---+
    // CMF : information on compression method
    // FLG : Additional information

    // Compressed data use the deflate algorithm
    // +----+----+----+----+----+================================+
    // |INFO|   LEN   |   NLEN  |... LEN bytes of literal data...|
    // +----+----+----+----+----+================================+
    // INFO : information on compression 
    // LEN : length of the data
    // NLEN : complement to 1 of LEN

    void WriteIDATChunk();

    // Code for CRC was taken from sample code on W3C website on PNG specification
    static uint32_t crc_table[256];
    static bool crc_table_computed;
    static uint32_t compute_crc(uint8_t *buf, size_t len);
    static uint32_t update_crc(uint32_t crc, uint8_t *buf, int len);
    static void compute_crc_table();

    // Code for Adler32 was taken from Wikipedia page of Adler32
    static const uint32_t MOD_ADLER = 65521;
    static uint32_t adler32(uint8_t *data, size_t len);


public:

    PngExporter(std::string filepath): m_filepath{filepath} {};
    int Export(int width, int height, std::shared_ptr<RGBColor[]> buffer) override;

};