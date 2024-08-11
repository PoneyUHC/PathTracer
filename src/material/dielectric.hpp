
#pragma once

#include "material/IMaterial.hpp"
 

class Dielectric : public IMaterial {

private:

    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double m_refraction_index;

    static double Reflectance(double cosine, double refraction_index);


public:

    Dielectric(double refraction_index): m_refraction_index{refraction_index} {};

    virtual bool Scatter(
        const Ray& incoming_ray, 
        const HitRecord& rec, 
        RGBColor& attenuation, 
        Ray& scattered_ray
    ) const override;

};