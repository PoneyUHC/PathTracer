
#pragma once

#include "vec.hpp"


inline int lerp(int start, int stop, double value)
{
    return int((1-value) * start + value * stop);
}


inline double lerp(double start, double stop, double value)
{
    return (1-value) * start + value * stop;
}


inline Vec3 lerp(Vec3 start, Vec3 stop, double value)
{
    return  start * (1-value) + stop * value;
}


inline double degrees_to_radians(double degrees)
{
    return degrees * M_PI / 180.0;
}

