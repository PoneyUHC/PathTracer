
#pragma once

#include "texture/ITexture.hpp"
#include "texture/solid_color_texture.hpp"

#include <memory>


class CheckerTexture : public ITexture {

private:

    double m_inv_scale;
    std::shared_ptr<ITexture> m_even_texture;
    std::shared_ptr<ITexture> m_odd_texture;


public:

    CheckerTexture(double scale, std::shared_ptr<ITexture> even, std::shared_ptr<ITexture> odd)
        : m_inv_scale{1.0 / scale}, m_even_texture{even}, m_odd_texture{odd} {}

    CheckerTexture(double scale, const RGBColor& even, const RGBColor& odd)
        : m_inv_scale{1.0 / scale}, 
        m_even_texture{std::make_shared<SolidColorTexture>(even)}, 
        m_odd_texture{std::make_shared<SolidColorTexture>(odd)} 
        {}


    RGBColor GetColor(double u, double v, const Point3& point) const override {
        int x_value = int(std::floor(m_inv_scale * point[0]));
        int y_value = int(std::floor(m_inv_scale * point[1]));
        int z_value = int(std::floor(m_inv_scale * point[2]));

        bool is_even = (x_value + y_value + z_value) % 2 == 0;

        return is_even ? m_even_texture->GetColor(u, v, point) : m_odd_texture->GetColor(u, v, point);
    } 

};