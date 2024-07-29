
#pragma once

#include "geometry/IHittable.hpp"

#include <memory>
#include <vector>


class HittableList : public IHittable {

private:

    std::vector<std::shared_ptr<IHittable>> m_objects;
    AABB m_aabb;

public:

    HittableList() {};
    HittableList(std::vector<std::shared_ptr<IHittable>> objects) : m_objects{objects} {};

    std::vector<std::shared_ptr<IHittable>> CopyObjects() const { return m_objects; }
    void Clear() { m_objects.clear(); }
    size_t Size() const { return m_objects.size(); }
    void AddObject(std::shared_ptr<IHittable> object);
    void AddObjects(std::vector<std::shared_ptr<IHittable>> objects);

    bool Hit(const Ray& ray, Interval& interval, HitRecord& outRecord) const override;

    AABB GetAABB() const override;

};