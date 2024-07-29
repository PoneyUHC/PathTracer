
#pragma once

#include <string>

#define XSTR(x) STR(x)
#define STR(x) #x


class Vec3;


int lerp(int start, int stop, double value);
double lerp(double start, double stop, double value);
Vec3 lerp(const Vec3& start, const Vec3& stop, double value);
double degrees_to_radians(double degrees);
double random_double();
double random_double(double min, double max);
int random_int(int min, int max);
Vec3 sample_in_unit_square();

std::string get_env_var(const std::string& key);
