
#pragma once

#include "renderer/IRenderer.hpp"


class Camera;
class HittableList;
class Ray;


struct PathTracingRendererParams {
    int aa_sample_per_pixel;
    int max_depth;
};


class PathTracingRenderer : public IRenderer {

private:

    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<HittableList> m_scene;
    std::shared_ptr<RGBColor[]> m_buffer;

    PathTracingRendererParams m_params;
    int m_render_step = 0;

    #ifdef _OPENMP
        void SetupOpenMP();
    #endif

public:

    PathTracingRenderer(
        std::shared_ptr<Camera> camera, 
        std::shared_ptr<HittableList> scene, 
        PathTracingRendererParams&& params
    );

    void Init() override;
    void Render(int n_steps) override;
    RGBColor GetRayColor(const Ray& ray, size_t depth);

    std::shared_ptr<RGBColor[]> GetBuffer() { return m_buffer; }
    PathTracingRendererParams GetParams() { return m_params; }

    void SetParams(PathTracingRendererParams&& params) { m_params = params; } 

};