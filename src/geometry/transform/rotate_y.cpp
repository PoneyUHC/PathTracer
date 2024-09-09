
#include "rotate_y.hpp"

#include "math/ray.hpp"


using namespace std;


RotateY::RotateY(shared_ptr<IHittable> object, double angle)
    : m_object{object}
{
    auto radians = degrees_to_radians(angle);
    m_sin_theta = sin(radians);
    m_cos_theta = cos(radians);
    m_aabb = object->GetAABB();

    Point3 min( INFINITY,  INFINITY,  INFINITY);
    Point3 max(-INFINITY, -INFINITY, -INFINITY);

    for (auto x : { m_aabb.X().Min(), m_aabb.X().Max() }) {
        for (auto y : { m_aabb.Y().Min(), m_aabb.Y().Max() }) {
            for (auto z : { m_aabb.Z().Min(), m_aabb.Z().Max() }) {

                Vec3 tester = Vec3 (
                    m_cos_theta*x + m_sin_theta*z,
                    y,
                    -m_sin_theta*x + m_cos_theta*z
                );

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    m_aabb = AABB(min, max);
}


bool RotateY::Hit(const Ray &ray, const Interval &interval, HitRecord &outRecord) const
{
    // Transform the ray from world space to object space.
    Vec3 origin = Point3(
        (m_cos_theta * ray.Origin()[0]) - (m_sin_theta * ray.Origin()[2]),
        ray.Origin()[1],
        (m_sin_theta * ray.Origin()[0]) + (m_cos_theta * ray.Origin()[2])
    );

    Vec3 direction = Vec3(
        (m_cos_theta * ray.Direction()[0]) - (m_sin_theta * ray.Direction()[2]),
        ray.Direction()[1],
        (m_sin_theta * ray.Direction()[0]) + (m_cos_theta * ray.Direction()[2])
    );

    Ray rotated_ray = Ray(origin, direction);

    // Determine whether an intersection exists in object space (and if so, where).
    if(!m_object->Hit(rotated_ray, interval, outRecord)){
        return false;
    }

    // Transform the intersection from object space back to world space.
    outRecord.hit_point = Point3(
        (m_cos_theta * outRecord.hit_point[0]) + (m_sin_theta * outRecord.hit_point[2]),
        outRecord.hit_point[1],
        (-m_sin_theta * outRecord.hit_point[0]) + (m_cos_theta * outRecord.hit_point[2])
    );

    outRecord.normal = Vec3(
        (m_cos_theta * outRecord.normal[0]) + (m_sin_theta * outRecord.normal[2]),
        outRecord.normal[1],
        (-m_sin_theta * outRecord.normal[0]) + (m_cos_theta * outRecord.normal[2])
    );

    return true;
}