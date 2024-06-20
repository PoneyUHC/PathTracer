
#include "ppm.hpp"

#include <string>
#include <iostream>
#include <fstream>


using namespace std;


int export_ppm(std::string filepath, int width, int height, RGBColor *buffer)
{
    ofstream file(filepath);
    if ( !file.is_open()) {
        cerr << __FUNCTION__ << " : failed to open " << filepath << endl;
        return 1;
    }

    file << "P3\n" << width << ' ' << height << "\n255\n";

    for(int j=0; j<width; ++j){
        for(int i=0; i<height; ++i){
            RGBColor color = buffer[i* width + j];
            file << color.r << ' ' << color.g << ' ' << color.b << '\n';
        }
    }

    return 0;
}