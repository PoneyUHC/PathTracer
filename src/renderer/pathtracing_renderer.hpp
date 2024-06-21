
#pragma once

#include "renderer/IRenderer.hpp"
#include "geometry/scene.hpp"
#include "camera.hpp"


class PathTracingRenderer : public IRenderer {

private:

    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<RGBColor[]> m_buffer;


public:

    PathTracingRenderer(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene);

    void Render() override;
    RGBColor GetRayColor(const Ray& ray);

    inline std::shared_ptr<RGBColor[]> GetBuffer() { return m_buffer; }

};