
#include "vec.hpp"


Vec3 Vec3::dot(const Vec3& other) 
{
    return Vec3{
        e[0]*other.e[0],
        e[1]*other.e[1],
        e[2]*other.e[2]
    }; 
}