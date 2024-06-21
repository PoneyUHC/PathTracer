
#pragma once

#include "vec.hpp"


class Ray {

private:

    Point3 m_origin;
    Vec3 m_direction;


public:

    Ray(const Point3& origin, const Vec3& direction) : m_origin{origin}, m_direction{direction} {};

    inline Point3 At(double time) const { return m_origin + m_direction * time; }

    inline Vec3 Direction() const { return m_direction; }
    inline Point3 Origin() const { return m_origin; }

};