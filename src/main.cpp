
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
    CameraParams camera_params;
    camera_params.aspect_ratio = 16.0 / 9.0;
    camera_params.image_width = width;
    camera_params.vfov = 20.0;
    camera_params.lookfrom = Point3(-2,2,1);
    camera_params.lookat   = Point3(0,0,-1);
    camera_params.vup      = Vec3(0,1,0);

    Point3 cameraPosition = Point3{0,0,0};

    return make_shared<Camera>(cameraPosition, std::move(camera_params));
}


shared_ptr<Scene> InitScene()
{
    auto material_ground = make_shared<Lambertian>(RGBColor(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(RGBColor(0.1, 0.2, 0.5));
    auto material_left   = make_shared<Dielectric>(1.50);
    auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<Metal>(RGBColor(0.8, 0.6, 0.2), 1.0);

    auto scene = make_shared<Scene>();
    scene->AddObjects({
        make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground),
        make_shared<Sphere>(Point3( 0.0,    0.0, -1.2),   0.5, material_center),
        make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left),
        make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.4, material_bubble),
        make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right)
    });

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