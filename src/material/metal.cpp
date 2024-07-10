
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
    scattered_ray = Ray(rec.hitPoint, reflected);
    attenuation = m_albedo;
    return true;
}