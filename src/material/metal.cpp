
#include "material/metal.hpp"

#include "geometry/IHittable.hpp"


bool Metal::Scatter(
    const Ray& incoming_ray, 
    const HitRecord& rec, 
    RGBColor& attenuation, 
    Ray& scattered_ray
) const 
{
    Vec3 reflected = incoming_ray.Direction().Reflect(rec.normal);
    reflected = reflected.Normalized() + (m_fuzziness * Vec3::RandomUnitVector());
    scattered_ray = Ray(rec.hitPoint, reflected);
    attenuation = m_albedo;
    return rec.normal.Dot(reflected) > 0;
}