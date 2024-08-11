
#include "sparsed_spheres_scene.hpp"

#include "geometry/sphere.hpp"
#include "geometry/hittable_list.hpp"
#include "geometry/bvh_node.hpp"
#include "material/lambertian.hpp"
#include "material/metal.hpp"
#include "material/dielectric.hpp"
#include "renderer/camera.hpp"
#include "renderer/pathtracing_renderer.hpp"


using namespace std;


shared_ptr<Camera> SparsedSpheresScene::InitCamera()
{
    CameraParams camera_params;
    camera_params.aspect_ratio = 16.0 / 9.0;
    camera_params.image_width = m_params.render_width;
    camera_params.vfov = 20.0;
    camera_params.lookfrom = Point3(13,2,3);
    camera_params.lookat = Point3(0,0,0);
    camera_params.vup = Vec3(0,1,0);
    camera_params.defocus_angle = 0.6;
    camera_params.focus_dist = 10.0;

    return make_shared<Camera>(std::move(camera_params));
}


shared_ptr<HittableList> SparsedSpheresScene::InitObjects()
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
                shared_ptr<IMaterial> sphere_material;

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
    
    if (m_params.enable_bvh){
        vector<shared_ptr<IHittable>> objects = hittable_list->CopyObjects();
        auto root = make_shared<BVHNode>(objects, 0, objects.size());

        hittable_list = make_shared<HittableList>();
        hittable_list->AddObject(root);
    }
    
    return hittable_list;
}


shared_ptr<PathTracingRenderer> SparsedSpheresScene::InitRenderer()
{
    PathTracingRendererParams params;
    params.aa_sample_per_pixel = 100;
    params.max_depth = 20;

    return make_shared<PathTracingRenderer>(m_camera, m_objets, move(params));
}


void SparsedSpheresScene::Build(SceneParams &&params)
{
    m_params = params;
    m_camera = InitCamera();
    m_objets = InitObjects();
    m_renderer = InitRenderer();
}


shared_ptr<RGBColor[]> SparsedSpheresScene::Render()
{
    m_renderer->Render();
    return m_renderer->GetBuffer();
}
