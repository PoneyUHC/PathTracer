
#pragma once

#include "vec.hpp"
#include "ray.hpp"
#include "interval.hpp"

#include <memory>


class IHittable;
class Material;

struct HitRecord {

    Point3 hitPoint;
    Vec3 normal;
    double t;
    bool front_face;
    std::shared_ptr<IHittable> object;
    std::shared_ptr<Material> material;


    void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
    {
        front_face = ray.Direction().Dot(outwardNormal) < 0.0;
        normal = front_face ? outwardNormal : -outwardNormal;
    }

};


// REFACTOR: create a subclass for IHittable (Mesh for example), that is a hittable that has a material
class IHittable {

public:

    virtual ~IHittable() {}
    virtual bool Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const = 0;

};