
#include "ppm.hpp"

#include "utils.hpp"

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

    for(int j=0; j<height; ++j){
        for(int i=0; i<width; ++i){
            RGBColor color = buffer[j* width + i];
            
            int x = int(255.999 * color.x());
            int y = int(255.999 * color.y());
            int z = int(255.999 * color.z());

            file << x << ' ' << y << ' ' << z << '\n';
        }
    }

    return 0;
}