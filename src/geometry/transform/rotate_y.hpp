
#pragma once


#include "geometry/IHittable.hpp"


class RotateY : public IHittable {

private:

    std::shared_ptr<IHittable> m_object;
    Vec3 m_offset;
    double m_cos_theta;
    double m_sin_theta;

    AABB m_aabb;


public:

    RotateY(std::shared_ptr<IHittable> object, double angle);

    bool Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const override;
    AABB GetAABB() const override { return m_aabb; }

};