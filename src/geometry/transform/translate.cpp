
#include "translate.hpp"

#include "math/ray.hpp"


Translate::Translate(std::shared_ptr<IHittable> object, Vec3 offset)
    : m_object{object}, m_offset{offset}
{
    m_aabb = object->GetAABB() + m_offset;
}


bool Translate::Hit(const Ray &ray, const Interval &interval, HitRecord &outRecord) const
{
    Ray offset_ray = Ray(ray.Origin() - m_offset, ray.Direction());

    if(!m_object->Hit(offset_ray, interval, outRecord)){
        return false;
    }

    outRecord.hit_point += m_offset;

    return true;
}