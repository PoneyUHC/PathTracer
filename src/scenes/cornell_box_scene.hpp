
#pragma once

#include "scenes/IScene.hpp"


class Camera;
class HittableList;
class PathTracingRenderer;

class CornellBoxScene: public IScene {

private:

    std::shared_ptr<Camera> InitCamera();
    std::shared_ptr<HittableList> InitObjects();
    std::shared_ptr<PathTracingRenderer> InitRenderer();


public:

    void Build(SceneParams&& params) override;
    std::shared_ptr<RGBColor[]> Render() override;

};