
#include "exporter/ppm.hpp"
#include "camera.hpp"
#include "utils.hpp"

#include <iostream>
#include <memory>


using namespace std;


RGBColor get_ray_color(const Ray& ray)
{
    Vec3 unitDirection = ray.Direction().Normalized();
    double a = 0.5 * (unitDirection.y() + 1.0);
    return lerp(RGBColor(1.0,1.0,1.0), RGBColor(0.5, 0.7, 1.0), a);
}


int main(int argc, char *argv[]){

    if (argc != 2){
        cout << "Usage : " << argv[0] << " width" << endl;
        return 1;
    }
    
    int width = atoi(argv[1]);
    

    Point3 cameraPosition = Point3{0,0,0};
    double aspectRatio = 16.0 / 9.0;
    double focalLength = 1.0;

    unique_ptr<Camera> camera = make_unique<Camera>(cameraPosition, aspectRatio, width, focalLength);
    int height = camera->ImageHeight();

    unique_ptr<RGBColor[]> image = make_unique<RGBColor[]>(width * height);

    for (int j = 0; j < height; j++) {
        std::clog << "\rScanlines remaining: " << (height - j) << ' ' << std::flush;
        for (int i = 0; i < width; i++) {
            Point3 pixelCenter = camera->GetPixelPosition(i, j).value();
            Vec3 rayDirection = pixelCenter - camera->CameraCenter();
            Ray ray = Ray(camera->CameraCenter(), rayDirection);

            image[j * width + i] = get_ray_color(ray);
        }
    }

    std::clog << "\rDone.\n";

    export_ppm("output/render.ppm", width, height, std::move(image));

    return 0;
}