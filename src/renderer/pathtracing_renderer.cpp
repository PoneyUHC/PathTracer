
#include "renderer/pathtracing_renderer.hpp"
#include "utils.hpp"

#include <iostream>


using namespace std;


PathTracingRenderer::PathTracingRenderer(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene)
    : m_camera{camera}, m_scene{scene}
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
            Point3 pixelCenter = m_camera->GetPixelPosition(i, j).value();
            Vec3 rayDirection = pixelCenter - m_camera->CameraCenter();
            Ray ray = Ray(m_camera->CameraCenter(), rayDirection);

            m_buffer[j * width + i] = GetRayColor(ray);
        }
    }
    
    std::clog << "\rDone.                 \n";
}


RGBColor PathTracingRenderer::GetRayColor(const Ray& ray)
{   
    HitRecord hitRecord;
    if( m_scene->Hit(ray, 0, INFINITY, hitRecord) ){
        return 0.5 * (hitRecord.normal + RGBColor(1,1,1));
    }

    Vec3 unitDirection = ray.Direction().Normalized();
    double a = 0.5 * (unitDirection.y() + 1.0);
    return lerp(RGBColor(1.0, 1.0, 1.0), RGBColor(0.5, 0.7, 1.0), a);
}