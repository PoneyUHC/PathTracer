
#pragma once

#include "renderer/IRenderer.hpp"


class Camera;
class Scene;
class Ray;


struct PathTracingRendererParams {
    int aa_sample_per_pixel;
    int max_depth;
};


class PathTracingRenderer : public IRenderer {

private:

    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<RGBColor[]> m_buffer;

    PathTracingRendererParams m_params;

    Ray SampleRayForPixel(int i, int j) const;


public:

    PathTracingRenderer(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, PathTracingRendererParams&& params);

    void Render() override;
    RGBColor GetRayColor(const Ray& ray, size_t depth);

    inline std::shared_ptr<RGBColor[]> GetBuffer() { return m_buffer; }

};