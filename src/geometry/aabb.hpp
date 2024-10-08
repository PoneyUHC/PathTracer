
#pragma once

#include "math/interval.hpp"
#include "math/vec.hpp"


class Ray;


class AABB {

public:

    Interval m_x, m_y, m_z;


public:

    AABB() {}
    AABB(const Interval& x, const Interval& y, const Interval& z);
    AABB(const Point3& a, const Point3& b);
    AABB(const AABB& aabb1, const AABB& aabb2);

    bool Hit(const Ray& ray, const Interval& interval) const;
    const Interval& IndexToInterval(int index) const;
    int LongestAxis() const;

    // To avoid intersection miss in case of exterme intervals, 
    // we pad the bounding boxes a bit
    void PadToMinimums();

    static const AABB Empty, Universe;
};