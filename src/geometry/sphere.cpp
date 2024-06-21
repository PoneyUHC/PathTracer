
#include "sphere.hpp"

#include <cmath>


using namespace std;


std::optional<Point3> Sphere::HitPoint(const Ray& ray)
{
    const Vec3 q = ray.Origin();
    const Vec3 d = ray.Direction();
    const Vec3 qc = m_center - q;
    const double a = d.Dot(d);
    const double b = -2 * d.Dot(qc);
    const double c = qc.Dot(qc) - m_radius*m_radius;

    const double discriminant = b*b - 4*a*c;

    if(discriminant < 0){
        return std::nullopt;
    } 
    
    const double firstRoot = (-b - sqrt(discriminant)) / 2*a;

    Point3 hitPoint = ray.At(firstRoot);
    return hitPoint;
}