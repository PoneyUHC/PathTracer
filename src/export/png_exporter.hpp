
#pragma once

#include "export/IExporter.hpp"

#include <string>
#include <fstream>


class PngExporter: public IExporter {

public:

    uint32_t m_width;
    uint32_t m_height;
    std::ofstream m_output_fs;

    // The DATA field from the IDAT chunk is the result of a zlib compression

    // ZLIB compression
    // +---+---+=====================+---+---+---+---+
    // |CMF|FLG|...compressed data...|    ADLER32    |
    // +---+---+=====================+---+---+---+---+
    // CMF : information on compression method
    // FLG : Additional information

    // Compressed data use the deflate algorithm
    // +---+---+---+---+---+---+================================+
    // | INFO  |  LEN  | NLEN  |... LEN bytes of literal data...|
    // +---+---+---+---+---+---+================================+
    // INFO : information on compression 
    // LEN : length of the data
    // NLEN : complement of LEN

    std::string m_filepath;

    static const uint8_t PNG_HEADER[];
    static const uint8_t IEND_CHUNK[];
    static const uint8_t IHDR_CHUNK_TYPE[];
    static const uint8_t IDAT_CHUNK_TYPE[];

    void write_IHDR_chunk();
    void write_IDAT_chunks(std::shared_ptr<RGBColor[]> color_buffer);

    // Code for CRC was taken from sample code on W3C website about PNG specification
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