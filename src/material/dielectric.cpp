
#include "dielectric.hpp"

#include "math/math_utils.hpp"
#include "math/vec.hpp"
#include "math/ray.hpp"
#include "geometry/IHittable.hpp"


using namespace std;

// dark magic I didn't understand about
// Schlick's approximation for reflectance.
double Dielectric::Reflectance(double cosine, double refraction_index)
{
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}


bool Dielectric::Scatter(
    const Ray &incoming_ray,
    const HitRecord &rec,
    RGBColor &attenuation,
    Ray &scattered_ray) const
{
    attenuation = RGBColor(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0/m_refraction_index) : m_refraction_index;

    Vec3 unit_direction = incoming_ray.Direction().Normalized();
    double cos_theta = fmin((-unit_direction).Dot(rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

    Vec3 direction;
    bool can_refract = refraction_ratio * sin_theta < 1.0;
    if (can_refract || Dielectric::Reflectance(cos_theta, refraction_ratio) > random_double()){
        direction = unit_direction.Refract(rec.normal, refraction_ratio);
    } else {
        direction = unit_direction.Reflect(rec.normal);
    }

    scattered_ray = Ray(rec.hitPoint, direction);
    return true;
}