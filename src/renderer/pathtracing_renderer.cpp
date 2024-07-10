
#include "renderer/pathtracing_renderer.hpp"
#include "utils.hpp"
#include "material/material.hpp"
#include "vec.hpp"

#include <iostream>


using namespace std;


PathTracingRenderer::PathTracingRenderer(
    std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, PathTracingRendererParams&& params
)
    : m_camera{camera}, m_scene{scene}, m_params{params}
{
    // shared_ptr initialization with size for array is C++20, we are C++17, workaround
    m_buffer = (shared_ptr<RGBColor[]>) make_unique<RGBColor[]>(camera->ImageWidth() * camera->ImageHeight());
}


void PathTracingRenderer::Render() {
    const int height = m_camera->ImageHeight();
    const int width = m_camera->ImageWidth();

    for (int j = 0; j < height; j++) {
        std::clog << "\rScanlines remaining: " << (height - j) << ' ' << std::flush;
        for (int i = 0; i < width; i++) {
            
            RGBColor accumulator(0,0,0);
            for(int sample = 0; sample < m_params.aa_sample_per_pixel; ++sample){
                Ray sampleRay = SampleRayForPixel(i, j);
                accumulator += GetRayColor(sampleRay, m_params.max_depth);
            }

            accumulator /= m_params.aa_sample_per_pixel; 
                
            m_buffer[j * width + i] = accumulator;
        }
    }
    
    std::clog << "\rDone.                 \n";
}


RGBColor PathTracingRenderer::GetRayColor(const Ray& ray, size_t depth)
{   
    if(depth == 0){
        return RGBColor(0, 0, 0);
    }

    HitRecord hitRecord;
    if( m_scene->Hit(ray, Interval(0.001, INFINITY), hitRecord) ){
        Ray scattered_ray;
        RGBColor attenuation(0, 0, 0);
        if (hitRecord.material->Scatter(ray, hitRecord, attenuation, scattered_ray)){
            return attenuation * GetRayColor(scattered_ray, depth-1);
        }
        return RGBColor(0, 0, 0);
    }

    Vec3 unitDirection = ray.Direction().Normalized();
    double a = 0.5 * (unitDirection.y() + 1.0);
    return lerp(RGBColor(1.0, 1.0, 1.0), RGBColor(0.5, 0.7, 1.0), a);
}


Ray PathTracingRenderer::SampleRayForPixel(int i, int j) const
{
    Vec3 samplePosition = m_camera->SamplePositionAroundPixel(i, j);
    return Ray(m_camera->CameraCenter(), samplePosition - m_camera->CameraCenter());
}