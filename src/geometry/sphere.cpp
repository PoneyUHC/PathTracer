
#include "sphere.hpp"

#include "math/ray.hpp"
#include "math/interval.hpp"

#include <cmath>


using namespace std;


Sphere::Sphere(const Point3& center, double radius, std::shared_ptr<IMaterial> material) 
    : m_center{center}, m_radius{radius}, m_material{material} 
{
    Vec3 aabb_corner = Vec3(m_radius, m_radius, m_radius);
    m_aabb = AABB(m_center - aabb_corner, m_center + aabb_corner);
}


bool Sphere::Hit(const Ray& ray, const Interval& interval, HitRecord& outRecord) const
{
    const Vec3 q = ray.Origin();
    const Vec3 d = ray.Direction();
    const Vec3 qc = m_center - q;
    const double a = d.Dot(d);
    // b = -2*h for simplified calculation
    const double h = d.Dot(qc);
    const double c = qc.MagnitudeSquared() - m_radius*m_radius;

    const double discriminant = h*h - a*c;

    if( discriminant < 0 ){
        return false;
    }
    
    const double sqrtd = sqrt(discriminant);
    double root = (h - sqrtd) / a;

    if ( root <= interval.Min() || root >= interval.Max() ){
        root = (h + sqrtd) / a;
        if( root <= interval.Min() || root >= interval.Max() ){
            return false;
        }
    }

    outRecord.t = root;
    outRecord.hit_point = ray.At(root);
    Vec3 outwardNormal = (outRecord.hit_point - m_center) / m_radius;
    outRecord.SetFaceNormal(ray, outwardNormal);
    get_sphere_uv(outwardNormal, outRecord.u, outRecord.v);
    outRecord.material = m_material;

    return true;
}


void Sphere::get_sphere_uv(const Point3 &point, double &u, double &v)
{
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    double theta = std::acos(-point[1]);
    double phi = std::atan2(-point[2], point[0]) + M_PI;

    u = phi / (2*M_PI);
    v = theta / M_PI;
}
