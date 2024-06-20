
#include "ppm.hpp"

#include "../utils.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>


using namespace std;
namespace fs = std::filesystem;


int export_ppm(std::string filepath, int width, int height, unique_ptr<RGBColor[]> buffer)
{
    fs::path path(filepath);
    fs::path parentDir = path.parent_path();
    if( !fs::exists(parentDir) ){
        fs::create_directory(parentDir);
    }

    ofstream file(filepath);
    if ( !file.is_open()) {
        cerr << __FUNCTION__ << " : failed to open " << filepath << endl;
        return 1;
    }

    file << "P3\n" << width << ' ' << height << "\n255\n";

    for(int j=0; j<width; ++j){
        for(int i=0; i<height; ++i){
            RGBColor color = buffer[i* width + j];
            
            int x = lerp(0, 255, color.x());
            int y = lerp(0, 255, color.y());
            int z = lerp(0, 255, color.z());

            file << x << ' ' << y << ' ' << z << '\n';
        }
    }

    return 0;
}