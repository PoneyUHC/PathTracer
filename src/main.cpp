
#include "exporter/ppm.hpp"
#include "camera.hpp"
#include "utils.hpp"

#include <iostream>
#include <memory>

RGBColor get_ray_color(const Ray& ray)
{
    Vec3 unitDirection = ray.Direction().normalized();
    double a = 0.5 * (unitDirection.y() + 1.0);
    return lerp(RGBColor(1.0,1.0,1.0), RGBColor(0.5, 0.7, 1.0), a);
}


using namespace std;

int main(int argc, char *argv[]){

    if (argc != 2){
        cout << "Usage : " << argv[0] << " width" << endl;
        return 1;
    }
    
    int width = atoi(argv[1]);

    Vec3 cameraPosition = Vec3{0,0,0};
    double aspectRatio = 16.0 / 9.0;
    double focalLength = 1.0;

    unique_ptr<Camera> camera = make_unique<Camera>(cameraPosition, aspectRatio, width, focalLength);
    int height = camera->m_image_height;

    unique_ptr<RGBColor[]> image = make_unique<RGBColor[]>(width * height);

    for (int i = 0; i < height; i++) {
        std::clog << "\rScanlines remaining: " << (height - i) << ' ' << std::flush;
        for (int j = 0; j < width; j++) {
            Vec3 pixelCenter = camera->GetPixelPosition(i, j).value();
            Vec3 rayDirection = pixelCenter - camera->m_camera_center;
            Ray ray = Ray(camera->m_camera_center, rayDirection);

            image[i * width + j] = get_ray_color(ray);
        }
    }

    std::clog << "\rDone.                 \n";

    export_ppm("output/render.ppm", width, height, std::move(image));

    return 0;

}