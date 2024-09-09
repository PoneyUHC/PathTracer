
#include "math/vec.hpp"
#include "math/ray.hpp"
#include "math/interval.hpp"
#include "math/math_utils.hpp"
#include "renderer/camera.hpp"
#include "renderer/pathtracing_renderer.hpp"
#include "geometry/hittable_list.hpp"
#include "material/IMaterial.hpp"
#include "utils.hpp"
#include "logger.hpp"

#include <omp.h>
#include <string>
#include <cstring>
#include <format>
#include "pathtracing_renderer.hpp"


using namespace std;


PathTracingRenderer::PathTracingRenderer(
    std::shared_ptr<Camera> camera, 
    std::shared_ptr<HittableList> scene, 
    PathTracingRendererParams&& params
)
    : m_camera{camera}, m_scene{scene}, m_params{params}
{
    // shared_ptr initialization with size for array is C++20, we are C++17, workaround
    m_buffer = make_shared<RGBColor[]>(camera->ImageWidth() * camera->ImageHeight());
}


void PathTracingRenderer::Render(int n_steps) {

    bool last_pass = false;
    if(n_steps >= m_params.aa_sample_per_pixel - m_render_step){
        n_steps = m_params.aa_sample_per_pixel - m_render_step;
        last_pass = true;
    }

    const int height = m_camera->ImageHeight();
    const int width = m_camera->ImageWidth();

    const int bar_width = 70;

    for(int sample = 0; sample < n_steps; ++sample){
        
        log_progress_bar(m_render_step, m_params.aa_sample_per_pixel, bar_width, false);
        ++m_render_step;

        #pragma omp parallel for shared(m_buffer, height, width, m_render_step)
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                Ray sampleRay = m_camera->SampleRayForPixel(i, j);
                RGBColor ray_color = GetRayColor(sampleRay, m_params.max_depth);
                m_buffer[j * width + i] = m_buffer[j * width + i] + (ray_color - m_buffer[j * width + i])/m_render_step;
            }
        }
    }
    
    if(last_pass){
        // for satisfaction of full bar
        log_progress_bar(m_render_step, m_params.aa_sample_per_pixel, bar_width, true);
        Logger::LogInfo("Done");
    }
}


void PathTracingRenderer::Init()
{
    m_render_step = 0;

    #ifdef _OPENMP
        SetupOpenMP();
    #endif

    memset((void*) m_buffer.get(), 0, sizeof(m_buffer));
}


#ifdef _OPENMP
void PathTracingRenderer::SetupOpenMP() 
{
    // assuming 6-8 physical cores on machine
    int num_threads = 12;
    string env_num_threads = get_env_var("OMP_NUM_THREAD");
    try {
        num_threads = stoi(env_num_threads);
    }
    catch (const std::exception& e){
        // keep default value
        Logger::LogWarning("OMP_NUM_THREAD is undefined");
    }

    omp_set_num_threads(num_threads);
    Logger::LogInfo(format("Using {} CPU threads to render", num_threads));
}
#endif


RGBColor PathTracingRenderer::GetRayColor(const Ray& ray, size_t depth)
{   
    if(depth <= 0){
        return BLACK;
    }

    HitRecord hit_record;
    static Interval interval = Interval(0.001, INFINITY);
    if(!m_scene->Hit(ray, interval, hit_record)){
        return m_camera->GetParams().background_color;
    }

    Ray scattered_ray;
    RGBColor attenuation = BLACK;
    RGBColor color_from_emission = hit_record.material->Emitted(hit_record.u, hit_record.v, hit_record.hit_point);

    if (!hit_record.material->Scatter(ray, hit_record, attenuation, scattered_ray)){
        return color_from_emission;
    }
    
    RGBColor color_from_scatter = attenuation * GetRayColor(scattered_ray, depth-1);

    return color_from_emission + color_from_scatter;
}