
#pragma once

#include <memory>

#include "material/IMaterial.hpp"
#include "texture/solid_color_texture.hpp"


class Lambertian : public IMaterial {

private:

    std::shared_ptr<ITexture> m_albedo;


public:

    Lambertian(const RGBColor& albedo): m_albedo{std::make_shared<SolidColorTexture>(albedo)} {};
    Lambertian(std::shared_ptr<ITexture> albedo): m_albedo{albedo} {};

    virtual bool Scatter(
        const Ray& incoming_ray, 
        const HitRecord& rec, 
        RGBColor& attenuation, 
        Ray& scattered_ray
    ) const override;

};