
#include "scenes/IScene.hpp"
#include "scenes/sparsed_spheres_scene.hpp"
#include "scenes/checkered_spheres_scene.hpp"
#include "scenes/solar_system_scene.hpp"
#include "scenes/quads_scene.hpp"
#include "scenes/cornell_box_scene.hpp"
#include "renderer/camera.hpp"
#include "renderer/pathtracing_renderer.hpp"
#include "export/png_exporter.hpp"
#include "logger.hpp"

#include <format>
#include <typeinfo>


using namespace std;


int main(int argc, char *argv[]){

    if (argc != 5){
        Logger::LogError(format("Usage : {} width aa_sample_per_pixel ray_max_depth scene_id", argv[0]));
        Logger::LogInfo("Advised values are width=1280 aa_sample_per_pixel=100 ray_max_depth=10 scene_id=5");
        return 1;
    }

    Logger::LogInfo("Initializing scene");

    SceneParams scene_params;
    int aa_sample_per_pixel;
    int max_depth;
    int scene_id;
    
    try{
        scene_params.render_width = stoi(argv[1]);
        scene_params.enable_bvh = true;

        aa_sample_per_pixel = stoi(argv[2]);
        max_depth = stoi(argv[3]);
        scene_id = stoi(argv[4]);
    }
    catch(std::exception const& e) {
        Logger::LogFatal("Could not parse command-line arguments");
        Logger::LogFatal(e.what());
        return 1;
    }
    

    unique_ptr<IScene> scene;

    switch(scene_id){
        case 1:
            scene = make_unique<SparsedSpheresScene>();
            break;
        case 2:
            scene = make_unique<CheckeredSpheresScene>();
            break;
        case 3:
            scene = make_unique<SolarSystemScene>();
            break;
        case 4:
            scene = make_unique<QuadsScene>();
            break;
        case 5:
            scene = make_unique<CornellBoxScene>();
            break;
        default:
            Logger::LogFatal("Problem in scene selection");
            return 1;
    }
    
    Logger::LogInfo("Building scene " + string(typeid(*scene.get()).name()));
    scene->Build(std::move(scene_params));

    Logger::LogInfo(format("Starting rendering scene {}", scene_id));
    Logger::LogInfo(format("Using {} samples per pixel, {} max rebounds", aa_sample_per_pixel, max_depth));
    uint32_t out_width = scene->GetCamera()->ImageWidth();
    uint32_t out_height = scene->GetCamera()->ImageHeight();
    Logger::LogInfo(format("Outputing image of size {}x{} pixels", out_width, out_height));
     
    auto renderer = dynamic_cast<PathTracingRenderer*>(scene->GetRenderer().get());
    PathTracingRendererParams params;
    params.aa_sample_per_pixel = aa_sample_per_pixel;
    params.max_depth = max_depth;
    renderer->SetParams(std::move(params));

    shared_ptr<RGBColor[]> color_buffer = scene->Render(params.aa_sample_per_pixel);

    PngExporter png_exporter("output/render.png");
    png_exporter.Export(out_width, out_height, color_buffer);

    return 0;
}