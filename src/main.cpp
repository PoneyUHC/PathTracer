
#include "export/ppm_exporter.hpp"
#include "renderer/pathtracing_renderer.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "geometry/sphere.hpp"
#include "geometry/scene.hpp"

#include <iostream>
#include <memory>


using namespace std;


int main(int argc, char *argv[]){

    if (argc != 2){
        cout << "Usage : " << argv[0] << " width" << endl;
        return 1;
    }
    
    int width = atoi(argv[1]);
    

    Point3 cameraPosition = Point3{0,0,0};
    double aspectRatio = 16.0 / 9.0;
    double focalLength = 1.0;

    auto camera = make_shared<Camera>(cameraPosition, aspectRatio, width, focalLength);
    int height = camera->ImageHeight();

    auto scene = make_shared<Scene>();
    auto sphere1 = make_shared<Sphere>(Point3(0,0,-1), 0.5);
    auto sphere2 = make_shared<Sphere>(Point3(0,-100.5,-1), 100);
    scene->AddObject(sphere1);
    scene->AddObject(sphere2);

    PathTracingRendererParams params;
    params.aa_sample_per_pixel = 20;
    params.max_depth = 50;
    PathTracingRenderer renderer(camera, scene, std::move(params));
    renderer.Render();

    PpmExporter ppmExporter("output/render.ppm");
    ppmExporter.Export(width, height, renderer.GetBuffer());

    return 0;
}