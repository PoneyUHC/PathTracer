
#pragma once

#include "math/vec.hpp"


class Ray;
class HitRecord;


class IMaterial {

public:

    virtual ~IMaterial() {}

    virtual bool Scatter(
        const Ray& incoming_ray, 
        const HitRecord& rec, 
        RGBColor& attenuation, 
        Ray& scattered_ray
    ) const
    {
        return false;
    }

    virtual RGBColor Emitted(double u, double v, const Point3& point) const
    {
        return BLACK;
    }


};