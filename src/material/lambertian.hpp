
#pragma once

#include "material/IMaterial.hpp"


class Lambertian : public IMaterial {

private:

    RGBColor m_albedo;


public:

    Lambertian(const RGBColor& albedo): m_albedo{albedo} {};

    virtual bool Scatter(
        const Ray& incoming_ray, 
        const HitRecord& rec, 
        RGBColor& attenuation, 
        Ray& scattered_ray
    ) const override;

};