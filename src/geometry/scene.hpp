
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

    inline void Clear() { m_objects.clear(); }
    inline void AddObject(std::shared_ptr<IHittable> object) { m_objects.push_back(object); }

    bool Hit(const Ray& ray, double tmin, double tmax, HitRecord& outRecord) const override;

};