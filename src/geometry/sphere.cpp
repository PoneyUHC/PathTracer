
#include "sphere.hpp"

#include <cmath>


using namespace std;


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
    const double min = interval.Min();
    const double max = interval.Max();

    if ( root <= min || root >= max ){
        root = (h + sqrtd) / a;
        if( root <= min || root >= max ){
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