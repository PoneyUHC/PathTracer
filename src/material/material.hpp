
#pragma once

#include "vec.hpp"


class Ray;
class HitRecord;


class Material {

public:

    virtual ~Material() {}
    virtual bool Scatter(
        const Ray& incoming_ray, 
        const HitRecord& rec, 
        RGBColor& attenuation, 
        Ray& scattered_ray
    ) const
    {
        return false;
    }


};