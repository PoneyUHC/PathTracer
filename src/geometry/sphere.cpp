
#include "sphere.hpp"

#include <cmath>


using namespace std;


bool Sphere::Hit(const Ray& ray, double tmin, double tmax, HitRecord& outRecord) const
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
    
    double sqrtd = sqrt(discriminant);
    double root = (h - sqrtd) / a;
    if ( root <= tmin || root >= tmax ){
        root = (h + sqrtd) / a;
        if( root <= tmin || root >= tmax ){
            return false;
        }
    }

    outRecord.t = root;
    outRecord.hitPoint = ray.At(root);
    outRecord.normal = (outRecord.hitPoint - m_center) / m_radius;

    return true;
}