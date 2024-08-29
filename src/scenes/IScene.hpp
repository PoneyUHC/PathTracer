
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


public:

    ~IScene() = default;
    virtual void Build(SceneParams&& params) = 0;
    virtual std::shared_ptr<RGBColor[]> Render() = 0;

    const Camera* GetCamera() { return m_camera.get(); }

};