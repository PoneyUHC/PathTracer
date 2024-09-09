#pragma once

#include "geometry/IHittable.hpp"
#include "geometry/hittable_list.hpp"

#include <memory>
#include <vector>


class BVHNode: public IHittable {

public:

    std::shared_ptr<IHittable> m_left;
    std::shared_ptr<IHittable> m_right;
    AABB m_aabb;

    static bool CompareAABB(std::shared_ptr<IHittable> obj1, std::shared_ptr<IHittable> obj2, int axis);


public:

    BVHNode(std::shared_ptr<HittableList> list) : BVHNode(list->GetObjects(), 0, list->GetObjects().size()) {
        // There's a C++ subtlety here. This constructor (without span indices) creates an
        // implicit copy of the hittable list, which we will modify. The lifetime of the copied
        // list only extends until this constructor exits. That's OK, because we only need to
        // persist the resulting bounding volume hierarchy.
    }
    BVHNode(std::vector<std::shared_ptr<IHittable>>& objects, size_t start, size_t end);

    bool Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const;

    AABB GetAABB() const override { return m_aabb; }

};