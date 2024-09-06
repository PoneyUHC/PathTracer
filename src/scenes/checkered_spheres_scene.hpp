
#pragma once

#include "scenes/IScene.hpp"


class Camera;
class HittableList;

class CheckeredSpheresScene: public IScene {

private:

    std::shared_ptr<Camera> InitCamera() override;
    std::shared_ptr<HittableList> InitObjects() override;
    std::shared_ptr<IRenderer> InitRenderer() override;

};