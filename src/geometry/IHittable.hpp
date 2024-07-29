
#pragma once

#include "vec.hpp"
#include "geometry/aabb.hpp"

#include <memory>


class IHittable;
class Material;
class Interval;
class Ray;


struct HitRecord {

    Point3 hitPoint;
    Vec3 normal;
    double t = 0.0;
    bool front_face = true;
    std::shared_ptr<IHittable> object;
    std::shared_ptr<Material> material;


    void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal);
};


// REFACTOR: create a subclass for IHittable (Mesh for example), that is a hittable that has a material
class IHittable {

public:

    virtual ~IHittable() {}

    virtual bool Hit(const Ray& ray, Interval& interval, HitRecord& outRecord) const = 0;

    virtual AABB GetAABB() const = 0;

};