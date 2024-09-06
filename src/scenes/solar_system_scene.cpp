
#include "solar_system_scene.hpp"

#include "geometry/hittable_list.hpp"
#include "geometry/sphere.hpp"
#include "material/lambertian.hpp"
#include "assets/image_asset.hpp"
#include "texture/image_texture.hpp"
#include "renderer/camera.hpp"
#include "renderer/pathtracing_renderer.hpp"

#include <memory>


using namespace std;


shared_ptr<Camera> SolarSystemScene::InitCamera()
{
    CameraParams camera_params;
    camera_params.aspect_ratio = 16.0 / 9.0;
    camera_params.image_width = m_params.render_width;
    camera_params.vfov = 40.0;
    camera_params.lookfrom = Point3(0,0,12);
    camera_params.lookat = Point3(0,0,0);
    camera_params.vup = Vec3(0,1,0);
    camera_params.defocus_angle = 0.0;
    camera_params.focus_dist = 10.0;
    camera_params.background_color = RGBColor(0.70, 0.80, 1.00);

    return make_shared<Camera>(std::move(camera_params));
}


shared_ptr<HittableList> SolarSystemScene::InitObjects()
{
    auto hittable_list = make_shared<HittableList>();

    auto earth_asset = make_shared<ImageAsset>("assets/2k_earth.jpg");
    earth_asset->Load();
    auto moon_asset = make_shared<ImageAsset>("assets/2k_moon.jpg");
    moon_asset->Load();
    auto sun_asset = make_shared<ImageAsset>("assets/2k_sun.jpg");
    sun_asset->Load();
    auto stars_asset = make_shared<ImageAsset>("assets/8k_stars_milky_way.jpg");
    stars_asset->Load();

    auto earth_texture = make_shared<ImageTexture>(earth_asset);
    auto earth_surface = make_shared<Lambertian>(earth_texture);

    auto moon_texture = make_shared<ImageTexture>(moon_asset);
    auto moon_surface = make_shared<Lambertian>(moon_texture);

    auto sun_texture = make_shared<ImageTexture>(sun_asset);
    auto sun_surface = make_shared<Lambertian>(sun_texture);
    
    auto stars_texture = make_shared<ImageTexture>(stars_asset);
    auto stars_surface = make_shared<Lambertian>(stars_texture);

    auto earth = make_shared<Sphere>(Point3(-3, 0, 0), 2, earth_surface);
    auto moon = make_shared<Sphere>(Point3(-4.5, 1, 3), 0.3, moon_surface);
    auto sun = make_shared<Sphere>(Point3(10, 3, -10), 8, sun_surface);
    auto stars = make_shared<Sphere>(Point3(0,0,-100), 70, stars_surface);

    hittable_list->AddObject(earth);
    hittable_list->AddObject(moon);
    hittable_list->AddObject(sun);
    hittable_list->AddObject(stars);

    return hittable_list;
}


shared_ptr<IRenderer> SolarSystemScene::InitRenderer()
{
    PathTracingRendererParams params;
    params.aa_sample_per_pixel = 100;
    params.max_depth = 10;
    m_renderer = make_shared<PathTracingRenderer>(m_camera, m_objets, move(params));
    m_renderer->Init();
    return m_renderer;
}