
#include "exporter/ppm.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "geometry/sphere.hpp"
#include "geometry/scene.hpp"

#include <iostream>
#include <memory>


using namespace std;


RGBColor get_ray_color(const Ray& ray, shared_ptr<Scene> scene)
{   
    HitRecord hitRecord;
    if( scene->Hit(ray, 0, INFINITY, hitRecord) ){
        return 0.5 * (hitRecord.normal + RGBColor(1,1,1));
    }

    Vec3 unitDirection = ray.Direction().Normalized();
    double a = 0.5 * (unitDirection.y() + 1.0);
    return lerp(RGBColor(1.0, 1.0, 1.0), RGBColor(0.5, 0.7, 1.0), a);
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

    auto camera = make_unique<Camera>(cameraPosition, aspectRatio, width, focalLength);
    int height = camera->ImageHeight();

    auto scene = make_shared<Scene>();
    auto sphere1 = make_shared<Sphere>(Point3(0,0,-1), 0.5);
    auto sphere2 = make_shared<Sphere>(Point3(0,-100.5,-1), 100);
    scene->AddObject(sphere1);
    scene->AddObject(sphere2);

    auto image = make_unique<RGBColor[]>(width * height);

    for (int j = 0; j < height; j++) {
        std::clog << "\rScanlines remaining: " << (height - j) << ' ' << std::flush;
        for (int i = 0; i < width; i++) {
            Point3 pixelCenter = camera->GetPixelPosition(i, j).value();
            Vec3 rayDirection = pixelCenter - camera->CameraCenter();
            Ray ray = Ray(camera->CameraCenter(), rayDirection);

            image[j * width + i] = get_ray_color(ray, scene);
        }
    }

    std::clog << "\rDone.                 \n";

    export_ppm("output/render.ppm", width, height, std::move(image));

    return 0;
}