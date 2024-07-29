
#pragma once

#include "geometry/IHittable.hpp"


class Material;


class Sphere : public IHittable {
    
private:

    Point3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;
    AABB m_aabb;


public:
    
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> material);

    bool Hit(const Ray& ray, Interval& interval, HitRecord& outRecord) const override;

    AABB GetAABB() const override { return m_aabb; }
    
};