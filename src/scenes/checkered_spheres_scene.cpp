
#include "checkered_spheres_scene.hpp"

#include "geometry/hittable_list.hpp"
#include "geometry/hittable_list.hpp"
#include "geometry/sphere.hpp"
#include "material/lambertian.hpp"
#include "texture/checker_texture.hpp"
#include "renderer/camera.hpp"
#include "renderer/pathtracing_renderer.hpp"

#include <memory>


using namespace std;


shared_ptr<Camera> CheckeredSpheresScene::InitCamera()
{
    CameraParams camera_params;
    camera_params.aspect_ratio = 16.0 / 9.0;
    camera_params.image_width = m_params.render_width;
    camera_params.vfov = 20.0;
    camera_params.lookfrom = Point3(13,2,3);
    camera_params.lookat = Point3(0,0,0);
    camera_params.vup = Vec3(0,1,0);
    camera_params.defocus_angle = 0.0;
    camera_params.focus_dist = 10.0;

    return make_shared<Camera>(std::move(camera_params));
}


shared_ptr<HittableList> CheckeredSpheresScene::InitObjects()
{
    auto hittable_list = make_shared<HittableList>();

    auto checker = make_shared<CheckerTexture>(
        0.32, 
        RGBColor(0.2, 0.3, 0.1), 
        RGBColor(0.9, 0.9, 0.9)
    );

    hittable_list->AddObject(
        make_shared<Sphere>(Point3(0,-10, 0), 10, make_shared<Lambertian>(checker))
    );

    hittable_list->AddObject(
        make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checker))
    );

    return hittable_list;
}


shared_ptr<IRenderer> CheckeredSpheresScene::InitRenderer()
{
    PathTracingRendererParams params;
    params.aa_sample_per_pixel = 100;
    params.max_depth = 20;
    params.background_color = RGBColor(0.70, 0.80, 1.00);

    return make_shared<PathTracingRenderer>(m_camera, m_objets, move(params));
}