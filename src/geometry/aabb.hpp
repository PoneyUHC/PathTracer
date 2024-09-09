
#pragma once

#include "math/interval.hpp"
#include "math/vec.hpp"


class Ray;


class AABB {

private:

    Interval m_x, m_y, m_z;


public:

    AABB() {}
    AABB(const Interval& x, const Interval& y, const Interval& z);
    AABB(const Point3& a, const Point3& b);
    AABB(const AABB& aabb1, const AABB& aabb2);

    Interval X() const { return m_x; }
    Interval Y() const { return m_y; }
    Interval Z() const { return m_z; }

    bool Hit(const Ray& ray, const Interval& interval) const;
    const Interval& IndexToInterval(int index) const;
    int LongestAxis() const;

    // To avoid intersection miss in case of exterme intervals, 
    // we pad the bounding boxes a bit
    void PadToMinimums();

    static const AABB Empty, Universe;
};

inline AABB operator+(const AABB& aabb, const Vec3& offset){ return AABB(aabb.X() + offset[0], aabb.Y() + offset[1], aabb.Z() + offset[2]); }
inline AABB operator+(const Vec3& offset, const AABB& aabb){ return aabb + offset; }