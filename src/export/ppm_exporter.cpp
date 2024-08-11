
#include "export/ppm_exporter.hpp"
#include "renderer/camera.hpp"
#include "math/interval.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>


using namespace std;
namespace fs = std::filesystem;


int PpmExporter::Export(int width, int height, std::shared_ptr<RGBColor[]> buffer)
{
    fs::path path(m_filepath);
    fs::path parentDir = path.parent_path();
    if( !fs::exists(parentDir) ){
        fs::create_directory(parentDir);
    }

    ofstream file(m_filepath);
    if ( !file.is_open()) {
        cerr << __FUNCTION__ << " : failed to open " << m_filepath << endl;
        return 1;
    }

    file << "P3\n" << width << ' ' << height << "\n255\n";

    for(int j=0; j<height; ++j){
        for(int i=0; i<width; ++i){

            RGBColor& color = buffer[j* width + i];
            uint8_t rgb[3];
            IExporter::rgb_normalized_to_8bits(color, rgb);

            file << rgb[0] << ' ' << rgb[1] << ' ' << rgb[2] << '\n';
        }
    }

    return 0;
}