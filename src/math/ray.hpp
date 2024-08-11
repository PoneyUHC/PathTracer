
#pragma once

#include "math/vec.hpp"


class Ray {

private:

    Point3 m_origin;
    Vec3 m_direction;


public:

    Ray() : m_origin{Point3(0,0,0)}, m_direction{Vec3(0,0,0)} {};
    Ray(const Point3& origin, const Vec3& direction) : m_origin{origin}, m_direction{direction} {};

    Point3 At(double time) const;

    Vec3 Direction() const { return m_direction; }
    Point3 Origin() const { return m_origin; }

};