
#pragma once

#include <cmath>

#include "utils.hpp"


class Vec3 {

public:

    double e[3];   

    Vec3() : e{0,0,0} {}
    Vec3(double x, double y, double z): e{x,y,z} {}

    inline double X() const { return e[0]; }
    inline double Y() const { return e[1]; }
    inline double Z() const { return e[2]; }

    inline Vec3 operator+(const Vec3& other) const { return Vec3{e[0]+other.e[0], e[1]+other.e[1], e[2]+other.e[2]}; }
    inline Vec3 operator-() const { return Vec3{-e[0], -e[1], -e[2]}; }
    inline Vec3 operator-(const Vec3& other) const { return Vec3{e[0]-other.e[0], e[1]-other.e[1], e[2]-other.e[2]}; }
    inline Vec3 operator*(double n) const { return Vec3{e[0]*n, e[1]*n, e[2]*n}; }
    inline Vec3 operator/(double n) const { return Vec3{e[0]/n, e[1]/n, e[2]/n}; }

    inline Vec3& operator+=(const Vec3& other) 
    { 
        this->e[0] += other.e[0];
        this->e[1] += other.e[1];
        this->e[2] += other.e[2];
        return *this;
    }

    inline Vec3& operator/=(double n) 
    { 
        this->e[0] /= n;
        this->e[1] /= n;
        this->e[2] /= n;
        return *this;
    }

    inline double Dot(const Vec3& other) const { return e[0]*other.e[0] + e[1]*other.e[1] + e[2]*other.e[2]; }
    inline double Magnitude() const { return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    inline double MagnitudeSquared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
    inline Vec3 Normalized() const { return *this / this->Magnitude(); }

    inline bool IsNearZero() const 
    {
        double s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    inline Vec3 Reflect(const Vec3& normal)
    {
        return *this - normal*2*this->Dot(normal);
    }
    
    
    // Requieres the current object to be a unit vector
    inline Vec3 RandomInSameHemisphere() const 
    {
        Vec3 other = RandomUnitVector();
        return other.Dot(*this) > 0.0 ? other : -other;
    }

    // Simple random in-square generation is not enough, 
    // as the distribution in the square would be uniform, 
    // but not the distribution of the projected vectors on unit-sphere
    // Thus, we only take vector generated inside the sphere.
    static inline Vec3 RandomUnitVector() 
    {
        Vec3 result;
        while(true){
            result = Vec3::Random(-1, 1);
            if(result.MagnitudeSquared() < 1.0){
                break;
            }
        }
        return result.Normalized();
    }

    static inline Vec3 Random() { return Vec3(random_double(), random_double(), random_double()); }
    static inline Vec3 Random(double min, double max) { return Vec3(random_double(min, max), random_double(min, max), random_double(min, max)); }
};


inline Vec3 operator*(double n, const Vec3& u) { return Vec3{n*u.e[0], n*u.e[1], n*u.e[2]}; }
inline Vec3 operator*(const Vec3& u, const Vec3& v) { return Vec3{u.e[0]*v.e[0], u.e[1]*v.e[1], u.e[2]*v.e[2]}; }


typedef Vec3 Point3;
typedef Vec3 RGBColor;