
#pragma once

#include <cmath>


class Vec3 {

public:

    double e[3];   

    Vec3() : e{0,0,0} {}
    Vec3(double x, double y, double z): e{x,y,z} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    Vec3 operator-() const { return Vec3{-e[0], -e[1], -e[2]}; }
    Vec3 operator-(const Vec3& other) const { return Vec3{e[0]-other.e[0], e[1]-other.e[1], e[2]-other.e[2]}; }
    Vec3 operator*(double n) const { return Vec3{e[0]*n, e[1]*n, e[2]*n}; }
    Vec3 operator/(double n) const { return Vec3{e[0]/n, e[1]/n, e[2]/n}; }
    Vec3 operator+(const Vec3& other) const { return Vec3{e[0]+other.e[0], e[1]+other.e[1], e[2]+other.e[2]}; }

    inline Vec3 Dot(const Vec3& other) { return Vec3{e[0]*other.e[0], e[1]*other.e[1], e[2]*other.e[2]}; }
    inline double Magnitude() const { return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    inline Vec3 Normalized() const { return *this / this->Magnitude(); }
};

typedef Vec3 Point3;
typedef Vec3 RGBColor;