
#pragma once

#include "math/vec.hpp"
#include "renderer/IRenderer.hpp"

#include <memory>


struct SceneParams {
    uint32_t render_width;
    bool enable_bvh;
};


class HittableList;
class Camera;
class IRenderer;

class IScene {

protected: 

    SceneParams m_params;
    std::shared_ptr<HittableList> m_objets;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<IRenderer> m_renderer;

    virtual std::shared_ptr<Camera> InitCamera() = 0;
    virtual std::shared_ptr<HittableList> InitObjects() = 0;
    virtual std::shared_ptr<IRenderer> InitRenderer() = 0;


public:

    ~IScene() = default;
    virtual void Build(SceneParams&& params)
    {
        m_params = params;
        m_camera = InitCamera();
        m_objets = InitObjects();
        m_renderer = InitRenderer();
    }

    virtual std::shared_ptr<RGBColor[]> Render(int n_steps)
    {
        m_renderer->Render(n_steps);
        return m_renderer->GetBuffer();
    }

    const std::shared_ptr<Camera> GetCamera() { return m_camera; }
    const std::shared_ptr<IRenderer> GetRenderer() { return m_renderer; }

};