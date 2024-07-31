#pragma once

#include "geometry/IHittable.hpp"

#include <memory>
#include <vector>


class BVHNode: public IHittable {

public:

    std::shared_ptr<IHittable> m_left;
    std::shared_ptr<IHittable> m_right;
    AABB m_aabb;

    static bool CompareAABB(std::shared_ptr<IHittable> obj1, std::shared_ptr<IHittable> obj2, int axis);


public:

    BVHNode(std::vector<std::shared_ptr<IHittable>>& objects, size_t start, size_t end);

    bool Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const;

    AABB GetAABB() const override { return m_aabb; }

};