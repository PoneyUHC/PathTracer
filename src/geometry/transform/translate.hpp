
#pragma once


#include "geometry/IHittable.hpp"


class Translate : public IHittable {

private:

    std::shared_ptr<IHittable> m_object;
    Vec3 m_offset;

    AABB m_aabb;


public:

    Translate(std::shared_ptr<IHittable> object, Vec3 offset);

    bool Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const override;
    AABB GetAABB() const override { return m_aabb; }

};