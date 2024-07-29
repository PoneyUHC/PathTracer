
#include "utils.hpp"

#include "vec.hpp"

#include <random>


int lerp(int start, int stop, double value)
{
    return int((1-value) * start + value * stop);
}


double lerp(double start, double stop, double value)
{
    return (1-value) * start + value * stop;
}


Vec3 lerp(const Vec3& start, const Vec3& stop, double value)
{
    return  start * (1-value) + stop * value;
}


double degrees_to_radians(double degrees)
{
    return degrees * M_PI / 180.0;
}


double random_double() 
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::random_device rd;
    static std::mt19937 generator(rd());
    return distribution(generator);
}


double random_double(double min, double max) 
{
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::random_device rd;
    static std::mt19937 generator(rd());
    return distribution(generator);
}


int random_int(int min, int max)
{
    return int(random_double(min, max));
}


Vec3 sample_in_unit_square() 
{ 
    return Vec3(random_double() - 0.5, random_double() - 0.5, 0); 
}


std::string get_env_var(const std::string& key)
{
    char* val = getenv(key.c_str());
    return val == NULL ? std::string("") : std::string(val);
}