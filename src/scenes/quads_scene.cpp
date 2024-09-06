
#include "quads_scene.hpp"

#include "geometry/hittable_list.hpp"
#include "geometry/quad.hpp"
#include "material/lambertian.hpp"
#include "renderer/camera.hpp"
#include "renderer/pathtracing_renderer.hpp"

#include <memory>


using namespace std;


shared_ptr<Camera> QuadsScene::InitCamera()
{
    CameraParams camera_params;
    camera_params.aspect_ratio = 1.0;
    camera_params.image_width = m_params.render_width;
    camera_params.vfov = 80.0;
    camera_params.lookfrom = Point3(0,0,9);
    camera_params.lookat = Point3(0,0,0);
    camera_params.vup = Vec3(0,1,0);
    camera_params.defocus_angle = 0.0;
    camera_params.focus_dist = 10.0;
    camera_params.background_color = RGBColor(0.70, 0.80, 1.00);

    return make_shared<Camera>(std::move(camera_params));
}


shared_ptr<HittableList> QuadsScene::InitObjects()
{
    auto hittable_list = make_shared<HittableList>();

    // Materials
    auto left_red     = make_shared<Lambertian>(RGBColor(1.0, 0.2, 0.2));
    auto back_green   = make_shared<Lambertian>(RGBColor(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<Lambertian>(RGBColor(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<Lambertian>(RGBColor(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<Lambertian>(RGBColor(0.2, 0.8, 0.8));

    // Quads
    hittable_list->AddObjects({
        make_shared<Quad>(Point3(-3,-2, 5), Vec3(0, 0,-4), Vec3(0, 4, 0), left_red),
        make_shared<Quad>(Point3(-2,-2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green),
        make_shared<Quad>(Point3( 3,-2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue),
        make_shared<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange),
        make_shared<Quad>(Point3(-2,-3, 5), Vec3(4, 0, 0), Vec3(0, 0,-4), lower_teal)
    });

    return hittable_list;
}


shared_ptr<IRenderer> QuadsScene::InitRenderer()
{
    PathTracingRendererParams params;
    m_renderer = make_shared<PathTracingRenderer>(m_camera, m_objets, move(params));
    m_renderer->Init();
    return m_renderer;
}

