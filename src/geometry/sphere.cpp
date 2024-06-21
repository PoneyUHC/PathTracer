
#include "sphere.hpp"

#include <cmath>


using namespace std;


std::optional<Point3> Sphere::HitPoint(const Ray& ray)
{
    const Vec3 q = ray.Origin();
    const Vec3 d = ray.Direction();
    const Vec3 qc = m_center - q;
    const double a = d.Dot(d);
    // b = -2*h for simplified calculation
    const double h = d.Dot(qc);
    const double c = qc.MagnitudeSquared() - m_radius*m_radius;

    const double discriminant = h*h - a*c;

    if(discriminant < 0){
        return std::nullopt;
    } 
    
    const double firstRoot = (h - sqrt(discriminant)) / a;

    Point3 hitPoint = ray.At(firstRoot);
    return hitPoint;
}