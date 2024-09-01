
#pragma once

#include <memory>

#include "material/IMaterial.hpp"
#include "texture/solid_color_texture.hpp"


class DiffuseLight : public IMaterial {

private:

    std::shared_ptr<ITexture> m_albedo;


public:

    DiffuseLight(const RGBColor& albedo): m_albedo{std::make_shared<SolidColorTexture>(albedo)} {};
    DiffuseLight(std::shared_ptr<ITexture> albedo): m_albedo{albedo} {};

    RGBColor Emitted(double u, double v, const Point3& point) const override
    {
        return m_albedo->GetColor(u, v, point);
    }

};