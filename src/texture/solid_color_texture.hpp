
#pragma once

#include "texture/ITexture.hpp"


class SolidColorTexture : public ITexture {

private:

    RGBColor m_albedo;


public:

    SolidColorTexture(const RGBColor& color): m_albedo{color} {}
    SolidColorTexture(double r, double g, double b): m_albedo{RGBColor(r, g, b)} {}

    RGBColor GetColor(double u, double v, const Point3& point) const override {
        return m_albedo;
    } 

};