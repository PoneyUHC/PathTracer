
#include "renderer/pathtracing_renderer.hpp"
#include "renderer/camera.hpp"
#include "math/math_utils.hpp"
#include "geometry/sphere.hpp"
#include "geometry/hittable_list.hpp"
#include "geometry/bvh_node.hpp"
#include "material/lambertian.hpp"
#include "material/metal.hpp"
#include "material/dielectric.hpp"
#include "export/png_exporter.hpp"

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


shared_ptr<HittableList> InitScene(bool enable_bvh)
{
    auto hittable_list = make_shared<HittableList>();

    auto ground_material = make_shared<Lambertian>(RGBColor(0.5, 0.5, 0.5));
    hittable_list->AddObject(
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
                    hittable_list->AddObject(
                        make_shared<Sphere>(center, 0.2, sphere_material)
                    );
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = RGBColor::Random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    hittable_list->AddObject(
                        make_shared<Sphere>(center, 0.2, sphere_material)
                    );
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    hittable_list->AddObject(
                        make_shared<Sphere>(center, 0.2, sphere_material)
                    );
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    hittable_list->AddObject(
        make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1)
    );

    auto material2 = make_shared<Lambertian>(RGBColor(0.4, 0.2, 0.1));
    hittable_list->AddObject(
        make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2)
    );

    auto material3 = make_shared<Metal>(RGBColor(0.7, 0.6, 0.5), 0.0);
    hittable_list->AddObject(
        make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3)
    );
    
    if (enable_bvh){
        vector<shared_ptr<IHittable>> objects = hittable_list->CopyObjects();
        auto root = make_shared<BVHNode>(objects, 0, objects.size());

        hittable_list = make_shared<HittableList>();
        hittable_list->AddObject(root);
    }
    
    return hittable_list;
}


PathTracingRenderer InitRenderer(shared_ptr<Camera> camera, shared_ptr<HittableList> scene)
{
    PathTracingRendererParams params;
    params.aa_sample_per_pixel = 100;
    params.max_depth = 20;

    return PathTracingRenderer(camera, scene, std::move(params));
}


int main(int argc, char *argv[]){

    if (argc != 2){
        cout << "Usage : " << argv[0] << " width" << endl;
        return 1;
    }

    cout << "Initializing scene" << endl;
    
    int width = atoi(argv[1]);

    bool enable_bvh = true;

    shared_ptr<Camera> camera = InitCamera(width);
    shared_ptr<HittableList> scene = InitScene(enable_bvh);

    PathTracingRenderer renderer = InitRenderer(camera, scene);

    cout << "Starting rendering" << endl;
    renderer.Render();

    PngExporter png_exporter("output/render.png");
    png_exporter.Export(camera->ImageWidth(), camera->ImageHeight(), renderer.GetBuffer());

}