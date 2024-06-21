
#pragma once

#include "vec.hpp"
#include "ray.hpp"


struct HitRecord {

    Point3 hitPoint;
    Vec3 normal;
    double t;

};


class IHittable {

public:

    virtual ~IHittable() {}
    virtual bool Hit(const Ray& ray, double tmin, double tmax, HitRecord& outRecord) const = 0;

};