
#include "scenes/IScene.hpp"
#include "scenes/sparsed_spheres_scene.hpp"
#include "scenes/checkered_spheres_scene.hpp"
#include "scenes/solar_system_scene.hpp"
#include "scenes/quads_scene.hpp"
#include "scenes/cornell_box_scene.hpp"
#include "renderer/camera.hpp"
#include "export/png_exporter.hpp"
#include "logger.hpp"

#include <format>
#include <typeinfo>


using namespace std;


int main(int argc, char *argv[]){

    if (argc != 2){
        Logger::LogError(format("Usage : {} width", argv[0]));
        return 1;
    }

    Logger::LogInfo("Initializing scene");
    
    SceneParams scene_params;
    scene_params.render_width = atoi(argv[1]);
    scene_params.enable_bvh = true;

    unique_ptr<IScene> scene;

    switch(3){
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

    Logger::LogInfo("Starting rendering");
    shared_ptr<RGBColor[]> color_buffer = scene->Render();

    PngExporter png_exporter("output/render.png");
    uint32_t out_width = scene->GetCamera()->ImageWidth();
    uint32_t out_height = scene->GetCamera()->ImageHeight();
    png_exporter.Export(out_width, out_height, color_buffer);

}