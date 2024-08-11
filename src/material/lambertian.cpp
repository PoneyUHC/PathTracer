
#include "lambertian.hpp"

#include "math/vec.hpp"
#include "math/ray.hpp"
#include "geometry/IHittable.hpp"


using namespace std;


bool Lambertian::Scatter(
    const Ray& incoming_ray, 
    const HitRecord& rec, 
    RGBColor& attenuation, 
    Ray& scattered_ray
) const 
{
    Vec3 scatter_direction = rec.normal + Vec3::RandomUnitVector();

    if (scatter_direction.IsNearZero()){
        scatter_direction = rec.normal;
    }

    scattered_ray = Ray(rec.hit_point, scatter_direction);
    attenuation = m_albedo;
    return true;
}