
#pragma once

#include "vec.hpp"

#include <random>


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


inline double random_double() 
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}


inline double random_double(double min, double max) 
{
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline Vec3 sample_in_unit_square() 
{ 
    return Vec3(random_double() - 0.5, random_double() - 0.5, 0); 
}
