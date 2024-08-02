
#include "math/ray.hpp"

#include "math/vec.hpp"


Point3 Ray::At(double time) const
{ 
    return m_origin + m_direction * time; 
}