
#pragma once

#include "scenes/IScene.hpp"


class Camera;
class HittableList;

class SolarSystemScene: public IScene {

private:

    std::shared_ptr<Camera> InitCamera();
    std::shared_ptr<HittableList> InitObjects();
    std::shared_ptr<IRenderer> InitRenderer();

};