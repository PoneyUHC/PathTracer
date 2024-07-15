
#include "export/ppm_exporter.hpp"
#include "renderer/pathtracing_renderer.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "geometry/sphere.hpp"
#include "geometry/scene.hpp"
#include "material/lambertian.hpp"
#include "material/metal.hpp"
#include "material/dielectric.hpp"

#include <iostream>
#include <memory>


using namespace std;


shared_ptr<Camera> InitCamera(int width)
{
    Point3 cameraPosition = Point3{0,0,0};
    double aspectRatio = 16.0 / 9.0;
    double focalLength = 1.0;

    return make_shared<Camera>(cameraPosition, aspectRatio, width, focalLength);
}


shared_ptr<Scene> InitScene()
{
    auto material_ground = make_shared<Lambertian>(RGBColor(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(RGBColor(0.1, 0.2, 0.5));
    auto material_left   = make_shared<Dielectric>(1.50);
    auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<Metal>(RGBColor(0.8, 0.6, 0.2), 1.0);

    auto sphere1 = make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground);
    auto sphere2 = make_shared<Sphere>(Point3( 0.0,    0.0, -1.2),   0.5, material_center);
    auto sphere3 = make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left);
    auto sphere4 = make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.4, material_bubble);
    auto sphere5 = make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right);

    auto scene = make_shared<Scene>();
    scene->AddObject(sphere1);
    scene->AddObject(sphere2);
    scene->AddObject(sphere3);
    scene->AddObject(sphere4);
    scene->AddObject(sphere5);

    return scene;
}


PathTracingRenderer InitRenderer(shared_ptr<Camera> camera, shared_ptr<Scene> scene)
{
    PathTracingRendererParams params;
    params.aa_sample_per_pixel = 20;
    params.max_depth = 10;

    return PathTracingRenderer(camera, scene, std::move(params));
}


int main(int argc, char *argv[]){

    if (argc != 2){
        cout << "Usage : " << argv[0] << " width" << endl;
        return 1;
    }
    
    int width = atoi(argv[1]);

    shared_ptr<Camera> camera = InitCamera(width);
    shared_ptr<Scene> scene = InitScene();

    PathTracingRenderer renderer = InitRenderer(camera, scene);
    renderer.Render();

    PpmExporter ppmExporter("output/render.ppm");
    ppmExporter.Export(camera->ImageWidth(), camera->ImageHeight(), renderer.GetBuffer());

    return 0;
}