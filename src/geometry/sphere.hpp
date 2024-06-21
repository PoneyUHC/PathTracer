
#pragma once

#include "geometry/IHittable.hpp"


class Sphere : public IHittable {
    
private:

    Point3 m_center;
    double m_radius;


public:
    
    Sphere(const Point3& center, double radius) : m_center{center}, m_radius{radius} {};

    bool Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const override;

};