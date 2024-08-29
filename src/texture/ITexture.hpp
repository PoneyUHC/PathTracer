
#pragma once

#include "math/vec.hpp"


class ITexture {

public:

    virtual ~ITexture() = default;
    virtual RGBColor GetColor(double u, double v, const Point3& point) const = 0;

};