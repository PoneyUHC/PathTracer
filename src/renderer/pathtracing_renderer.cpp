
#include "math/vec.hpp"
#include "math/ray.hpp"
#include "math/interval.hpp"
#include "math/math_utils.hpp"
#include "renderer/camera.hpp"
#include "renderer/pathtracing_renderer.hpp"
#include "geometry/hittable_list.hpp"
#include "material/IMaterial.hpp"
#include "utils.hpp"

#include <omp.h>
#include <iostream>
#include <string>


using namespace std;


PathTracingRenderer::PathTracingRenderer(
    std::shared_ptr<Camera> camera, 
    std::shared_ptr<HittableList> scene, 
    PathTracingRendererParams&& params
)
    : m_camera{camera}, m_scene{scene}, m_params{params}
{
    // shared_ptr initialization with size for array is C++20, we are C++17, workaround
    m_buffer = (shared_ptr<RGBColor[]>) make_unique<RGBColor[]>(camera->ImageWidth() * camera->ImageHeight());
}


void PathTracingRenderer::Render() {
    const int height = m_camera->ImageHeight();
    const int width = m_camera->ImageWidth();

    
    #ifdef _OPENMP
        // assuming 6-8 physical cores on machine
        int num_threads = 12;
        string env_num_threads = get_env_var("OMP_NUM_THREAD");
        try {
            num_threads = stoi(env_num_threads);
        }
        catch (const std::exception& e){
            // keep default value
            cout << "OMP_NUM_THREAD is undefined" << endl;
        }

        omp_set_num_threads(num_threads);
        cout << "Using " << num_threads << " CPU threads to render" << endl;

        int progress = 0;
        #pragma omp parallel for num_threads(num_threads) shared(progress)
    #endif

    for (int j = 0; j < height; j++) {

        #ifdef _OPENMP
            #pragma omp critical
            {
                clog << "\rLines remaining: " << (height - progress) << ' ' << flush;
            }
        #else
            clog << "\rLines remaining: " << (height - j) << ' ' << flush;
        #endif

        for (int i = 0; i < width; i++) {
            
            RGBColor accumulator(0,0,0);
            for(int sample = 0; sample < m_params.aa_sample_per_pixel; ++sample){
                Ray sampleRay = m_camera->SampleRayForPixel(i, j);
                accumulator += GetRayColor(sampleRay, m_params.max_depth);
            }

            accumulator /= m_params.aa_sample_per_pixel; 
                
            m_buffer[j * width + i] = accumulator;
        }

        #ifdef _OPENMP
            #pragma omp critical
            {
                ++progress;
            }
        #endif
    }
    
    clog << "\rDone.                 \n";
}


RGBColor PathTracingRenderer::GetRayColor(const Ray& ray, size_t depth)
{   
    if(depth == 0){
        return RGBColor(0, 0, 0);
    }

    HitRecord hitRecord;
    Interval interval = Interval(0.001, INFINITY);
    if( m_scene->Hit(ray, interval, hitRecord) ){
        Ray scattered_ray;
        RGBColor attenuation(0, 0, 0);
        if (hitRecord.material->Scatter(ray, hitRecord, attenuation, scattered_ray)){
            return attenuation * GetRayColor(scattered_ray, depth-1);
        }
        return RGBColor(0, 0, 0);
    }

    Vec3 unitDirection = ray.Direction().Normalized();
    double a = 0.5 * (unitDirection[1] + 1.0);
    return lerp(RGBColor(1.0, 1.0, 1.0), RGBColor(0.5, 0.7, 1.0), a);
}