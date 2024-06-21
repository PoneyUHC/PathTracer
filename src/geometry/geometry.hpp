
#pragma once

#include "vec.hpp"
#include "ray.hpp"

#include <optional>


class Geometry {

public:

    virtual ~Geometry() {}
    virtual std::optional<Point3> HitPoint(const Ray& ray) = 0;

};