
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
    camera_params.focal_length = 1.0;
    camera_params.image_width = width;
    camera_params.vfov = 90.0;

    Point3 cameraPosition = Point3{0,0,0};

    return make_shared<Camera>(cameraPosition, std::move(camera_params));
}


shared_ptr<Scene> InitScene()
{
    double R = cos(M_PI/4);

    auto material_left  = make_shared<Lambertian>(RGBColor(0,0,1));
    auto material_right = make_shared<Lambertian>(RGBColor(1,0,0));

    auto scene = make_shared<Scene>();
    scene->AddObjects({
            make_shared<Sphere>(Point3(-R, 0, -1), R, material_left),
            make_shared<Sphere>(Point3( R, 0, -1), R, material_right)
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