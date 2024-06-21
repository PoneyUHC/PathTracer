
#pragma once

#include "vec.hpp"
#include "ray.hpp"


struct HitRecord {

    Point3 hitPoint;
    Vec3 normal;
    double t;
    bool front_face;


    void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
    {
        front_face = ray.Direction().Dot(outwardNormal) < 0;
        normal = front_face ? normal : -normal;
    }

};


class IHittable {

public:

    virtual ~IHittable() {}
    virtual bool Hit(const Ray& ray, double tmin, double tmax, HitRecord& outRecord) const = 0;

};