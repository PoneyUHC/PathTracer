
#pragma once

#include "geometry/IHittable.hpp"


class Material;


class Sphere : public IHittable {
    
private:

    Point3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;


public:
    
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> material) : m_center{center}, m_radius{radius}, m_material{material} {};

    bool Hit(const Ray& ray, Interval& interval, HitRecord& outRecord) const override;

};