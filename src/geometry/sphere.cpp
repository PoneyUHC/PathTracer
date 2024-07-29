
#include "sphere.hpp"

#include "ray.hpp"
#include "interval.hpp"

#include <cmath>


using namespace std;


Sphere::Sphere(const Point3& center, double radius, std::shared_ptr<Material> material) 
    : m_center{center}, m_radius{radius}, m_material{material} 
{
    Vec3 aabb_corner = Vec3(m_radius, m_radius, m_radius);
    m_aabb = AABB(m_center - aabb_corner, m_center + aabb_corner);
}


bool Sphere::Hit(const Ray& ray, Interval& interval, HitRecord& outRecord) const
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
    outRecord.hitPoint = ray.At(root);
    Vec3 outwardNormal = (outRecord.hitPoint - m_center) / m_radius;
    outRecord.SetFaceNormal(ray, outwardNormal);
    outRecord.material = m_material;

    return true;
}