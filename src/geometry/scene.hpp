
#pragma once

#include "geometry/IHittable.hpp"

#include <memory>
#include <vector>


class Scene : public IHittable {

private:

    std::vector<std::shared_ptr<IHittable>> m_objects;

public:

    Scene() {};
    Scene(std::vector<std::shared_ptr<IHittable>> objects) : m_objects{objects} {};

    void Clear() { m_objects.clear(); }
    void AddObject(std::shared_ptr<IHittable> object) { m_objects.push_back(object); }
    void AddObjects(std::vector<std::shared_ptr<IHittable>> objects) 
    { 
        m_objects.insert(m_objects.end(), objects.begin(), objects.end()); 
    }

    bool Hit(const Ray& ray, Interval& interval, HitRecord& outRecord) const override;

};