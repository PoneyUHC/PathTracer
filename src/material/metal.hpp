
#pragma once

#include "material/material.hpp"
 

class Metal : public Material {

private:

    RGBColor m_albedo;
    double m_fuzziness;


public:

    Metal(const RGBColor& albedo, double fuzziness): 
        m_albedo{albedo}, m_fuzziness{fuzziness < 1 ? fuzziness : 1} {};

    virtual bool Scatter(
        const Ray& incoming_ray, 
        const HitRecord& rec, 
        RGBColor& attenuation, 
        Ray& scattered_ray
    ) const override;

};