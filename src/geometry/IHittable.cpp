
#include "geometry/IHittable.hpp"

#include "ray.hpp"


void HitRecord::SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
{
    front_face = ray.Direction().Dot(outwardNormal) < 0.0;
    normal = front_face ? outwardNormal : -outwardNormal;
}