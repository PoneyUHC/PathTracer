
#include "image_asset.hpp"

#include "utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#include <memory>
#include <filesystem>
#include <iostream>


using namespace std;
namespace fs = std::filesystem;


int ImageAsset::Load()
{
    clog << "Loading " << m_filepath << " ..." << endl;
    fs::path path(m_filepath);

    if(!fs::exists(m_filepath)){
        cerr << "\t" << m_filepath << ": file not found" << endl;
        return false;
    }

    int n = m_bytes_per_pixel;

    float* fdata;
    fdata = stbi_loadf(path.c_str(), &m_width, &m_height, &n, m_bytes_per_pixel);
    if(fdata == NULL){
        cerr << "\t" << m_filepath << ": error while loading file" << endl;
        cerr << "\t" << stbi_failure_reason() << endl;
        return false;
    }

    m_fdata = unique_ptr<float[]>(fdata);

    int total_bytes = m_width * m_height * m_bytes_per_pixel;
    m_bdata = make_unique<uint8_t[]>(total_bytes);

    #pragma omp parallel for
    for(int i=0; i<total_bytes; ++i){
        m_bdata[i] = rgb_normalized_to_8bits(m_fdata[i]);
    }

    clog << "Loaded " << m_filepath << " successfully" << endl;
    return true;
}


float* ImageAsset::GetPixelData(int x, int y)
{
    if(m_fdata == nullptr){
        return MAGENTA;
    }
    return m_fdata.get() + y*m_bytes_per_pixel*m_width + x*m_bytes_per_pixel;
}
