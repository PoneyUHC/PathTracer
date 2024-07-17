
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
    camera_params.lookfrom = Point3(13,2,3);
    camera_params.lookat = Point3(0,0,0);
    camera_params.vup = Vec3(0,1,0);
    camera_params.defocus_angle = 0.6;
    camera_params.focus_dist = 10.0;

    return make_shared<Camera>(std::move(camera_params));
}


shared_ptr<Scene> InitScene()
{
    auto scene = make_shared<Scene>();

    auto ground_material = make_shared<Lambertian>(RGBColor(0.5, 0.5, 0.5));
    scene->AddObject(
        make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_material)
    );

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).Magnitude() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = RGBColor::Random(0,1) * RGBColor::Random(0,1);
                    sphere_material = make_shared<Lambertian>(albedo);
                    scene->AddObject(
                        make_shared<Sphere>(center, 0.2, sphere_material)
                    );
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = RGBColor::Random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    scene->AddObject(
                        make_shared<Sphere>(center, 0.2, sphere_material)
                    );
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    scene->AddObject(
                        make_shared<Sphere>(center, 0.2, sphere_material)
                    );
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    scene->AddObject(
        make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1)
    );

    auto material2 = make_shared<Lambertian>(RGBColor(0.4, 0.2, 0.1));
    scene->AddObject(
        make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2)
    );

    auto material3 = make_shared<Metal>(RGBColor(0.7, 0.6, 0.5), 0.0);
    scene->AddObject(
        make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3)
    );

    return scene;
}


PathTracingRenderer InitRenderer(shared_ptr<Camera> camera, shared_ptr<Scene> scene)
{
    PathTracingRendererParams params;
    params.aa_sample_per_pixel = 500;
    params.max_depth = 50;

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