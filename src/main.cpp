#include "color.hpp"
#include "exporter/ppm.hpp"

#include <iostream>
#include <memory>


using namespace std;

int main(int argc, char *argv[]){

    if (argc != 3){
        cout << "Usage : " << argv[0] << " width height" << endl;
        return 1;
    }
    
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    unique_ptr<RGBColor[]> image = make_unique<RGBColor[]>(width * height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            auto r = double(i) / (width-1);
            auto g = double(j) / (height-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            image[i * width + j] = RGBColor{ir, ig, ib};
        }
    }

    export_ppm("output/render.ppm", width, height, std::move(image));

    return 0;

}