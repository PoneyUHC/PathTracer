
#pragma once

#include "geometry/IHittable.hpp"


class IMaterial;


class Sphere : public IHittable {
    
private:

    Point3 m_center;
    double m_radius;

    AABB m_aabb;
    std::shared_ptr<IMaterial> m_material;


public:
    
    Sphere(const Point3& center, double radius, std::shared_ptr<IMaterial> material);

    bool Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const override;

    AABB GetAABB() const override { return m_aabb; }
    
    static void get_sphere_uv(const Point3& point, double& u, double& v);
};