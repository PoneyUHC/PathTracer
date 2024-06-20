#pragma once

#include "vec.hpp"

class Ray {

public:

    Point3 m_origin;
    Vec3 m_direction;

    Ray(const Point3& origin, const Vec3& direction) : m_origin{origin}, m_direction{direction} {};

    Point3 at(double time) const { return m_origin + m_direction * time; }

    Vec3 Direction() const { return m_direction; }
    Point3 Origin() const { return m_origin; }

};