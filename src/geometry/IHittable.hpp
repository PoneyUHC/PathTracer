
#pragma once

#include "vec.hpp"
#include "ray.hpp"
#include "interval.hpp"


struct HitRecord {

    Point3 hitPoint;
    Vec3 normal;
    double t;
    bool front_face;


    void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
    {
        front_face = ray.Direction().Dot(outwardNormal) < 0;
        normal = front_face ? outwardNormal : -outwardNormal;
    }

};


class IHittable {

public:

    virtual ~IHittable() {}
    virtual bool Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const = 0;

};