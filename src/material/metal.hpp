
#pragma once

#include "material/material.hpp"
 

class Metal : public Material {

private:

    RGBColor m_albedo;


public:

    Metal(const RGBColor& albedo): m_albedo{albedo} {};

    virtual bool Scatter(
        const Ray& incoming_ray, 
        const HitRecord& rec, 
        RGBColor& attenuation, 
        Ray& scattered_ray
    ) const override;

};