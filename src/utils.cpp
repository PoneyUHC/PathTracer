#include "utils.hpp"

int lerp(int start, int stop, double value)
{
    return int((1-value) * start + value * stop);
}

double lerp(double start, double stop, double value)
{
    return (1-value) * start + value * stop;
}

Vec3 lerp(Vec3 start, Vec3 stop, double value)
{
    return  start * (1-value) + stop * value;
}

