
#pragma once

#include "assets/IAsset.hpp"


class ImageAsset : public IAsset {

private:

    std::string m_filepath;
    int m_width = 0;
    int m_height = 0;
    int m_bytes_per_pixel = 3;

    std::unique_ptr<float[]> m_fdata;
    std::unique_ptr<uint8_t[]> m_bdata;


public:

    ImageAsset(std::string filepath): m_filepath{filepath} {};
    
    int Load() override;
    std::string GetPath() const override { return m_filepath; }

    int Width() const { return (m_fdata == nullptr) ? 0 : m_width; }
    int Height() const { return (m_fdata == nullptr) ? 0 : m_height; }
    float* GetPixelData(int x, int y);

};