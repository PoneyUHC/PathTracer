
#pragma once


#include "geometry/IHittable.hpp"

#include "math/vec.hpp"


class Quad : public IHittable {

private:

    Point3 m_q;
    Vec3 m_u;
    Vec3 m_v;

    Vec3 m_normal;
    // d is the intersection equation result for the plan
    // for a point Q on the plane, n.Q = d
    double m_d;
    // w is a constant for a given quadrilateral
    // It simplifies the check to know if we are inside the quad
    Vec3 m_w;

    AABB m_aabb;
    std::shared_ptr<IMaterial> m_material;

    bool IsInQuad(double alpha, double beta, HitRecord& hit_record) const;


public:

    Quad(const Point3& q, const Vec3& u, const Vec3& v, std::shared_ptr<IMaterial> material);

    bool Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const override;
    AABB GetAABB() const override { return m_aabb; }
};