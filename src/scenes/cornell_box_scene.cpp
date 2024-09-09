
#include "cornell_box_scene.hpp"

#include "geometry/hittable_list.hpp"
#include "geometry/hittable_list.hpp"
#include "geometry/quad.hpp"
#include "geometry/bvh_node.hpp"
#include "geometry/transform/translate.hpp"
#include "geometry/transform/rotate_y.hpp"
#include "material/lambertian.hpp"
#include "material/diffuse_light.hpp"
#include "texture/checker_texture.hpp"
#include "renderer/camera.hpp"
#include "renderer/pathtracing_renderer.hpp"

#include <memory>


using namespace std;


shared_ptr<Camera> CornellBoxScene::InitCamera()
{
    CameraParams camera_params;
    camera_params.aspect_ratio = 1.0;
    camera_params.image_width = m_params.render_width;
    camera_params.vfov = 40.0;
    camera_params.lookfrom = Point3(278, 278, -800);
    camera_params.lookat = Point3(278, 278, 0);
    camera_params.vup = Vec3(0,1,0);
    camera_params.defocus_angle = 0.0;
    camera_params.focus_dist = 10.0;
    camera_params.background_color = BLACK;

    return make_shared<Camera>(std::move(camera_params));
}


shared_ptr<HittableList> CornellBoxScene::InitObjects()
{
    auto hittable_list = make_shared<HittableList>();

    auto red   = make_shared<Lambertian>(RGBColor(.65, .05, .05));
    auto white = make_shared<Lambertian>(RGBColor(.73, .73, .73));
    auto green = make_shared<Lambertian>(RGBColor(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(RGBColor(15, 15, 15));

    shared_ptr<IHittable> box1 = Quad::Box(Point3(0,0,0), Point3(165,330,165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265,0,295));

    shared_ptr<IHittable> box2 = Quad::Box(Point3(0,0,0), Point3(165,165,165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130,0,65));

    hittable_list->AddObjects({
        make_shared<Quad>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), green),
        make_shared<Quad>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), red),
        make_shared<Quad>(Point3(343, 554, 332), Vec3(-130,0,0), Vec3(0,0,-105), light),
        make_shared<Quad>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), white),
        make_shared<Quad>(Point3(555,555,555), Vec3(-555,0,0), Vec3(0,0,-555), white),
        make_shared<Quad>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), white),
        box1,
        box2
    });

    if(m_params.enable_bvh) {
        auto root = make_shared<BVHNode>(hittable_list);

        hittable_list = make_shared<HittableList>();
        hittable_list->AddObject(root);
    }

    return hittable_list;
}


shared_ptr<IRenderer> CornellBoxScene::InitRenderer()
{
    PathTracingRendererParams params;
    params.aa_sample_per_pixel = 100;
    params.max_depth = 10;
    m_renderer = make_shared<PathTracingRenderer>(m_camera, m_objets, move(params));
    m_renderer->Init();
    return m_renderer;
}