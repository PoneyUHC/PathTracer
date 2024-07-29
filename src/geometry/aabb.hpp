
#pragma once

#include "interval.hpp"
#include "vec.hpp"


class Ray;


class AABB {

public:

    Interval m_x, m_y, m_z;


public:

    AABB() {}
    AABB(const Interval& x, const Interval& y, const Interval& z): m_x{x}, m_y{y}, m_z{z} {}
    AABB(const Point3& a, const Point3& b);
    AABB(const AABB& aabb1, const AABB& aabb2);

    bool Hit(const Ray& ray, Interval& interval) const;
    const Interval& IndexToInterval(int index) const;
    int LongestAxis() const;

    static const AABB Empty, Universe;
};