
#pragma once

#include "geometry/geometry.hpp"


class Sphere : public Geometry {
    
private:

    Point3 m_center;
    double m_radius;


public:
    
    Sphere(const Point3& center, double radius) : m_center{center}, m_radius{radius} {};

    std::optional<Point3> HitPoint(const Ray& ray) override;

};