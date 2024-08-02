
#include "export/ppm_exporter.hpp"
#include "renderer/camera.hpp"
#include "math/interval.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>


using namespace std;
namespace fs = std::filesystem;


int PpmExporter::Export(int width, int height, std::shared_ptr<RGBColor[]> buffer) const
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

            RGBColor color = buffer[j* width + i];
            double r = color.X();
            double g = color.Y();
            double b = color.Z();

            r = linear_to_gamma(r);
            g = linear_to_gamma(g);
            b = linear_to_gamma(b);
            
            static const Interval intensity(0.000, 0.999);
            int rbyte = int(256 * intensity.Clamp(r));
            int gbyte = int(256 * intensity.Clamp(g));
            int bbyte = int(256 * intensity.Clamp(b));

            file << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
        }
    }

    return 0;
}