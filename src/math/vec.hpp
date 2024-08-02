
#pragma once

#include <cmath>

#include "math/math_utils.hpp"


class Vec3 {

public:

    double e[3];   

    Vec3() : e{0,0,0} {}
    Vec3(double x, double y, double z): e{x,y,z} {}

    double X() const { return e[0]; }
    double Y() const { return e[1]; }
    double Z() const { return e[2]; }

    Vec3 operator+(const Vec3& other) const { return Vec3{e[0]+other.e[0], e[1]+other.e[1], e[2]+other.e[2]}; }
    Vec3 operator-() const { return Vec3{-e[0], -e[1], -e[2]}; }
    Vec3 operator-(const Vec3& other) const { return Vec3{e[0]-other.e[0], e[1]-other.e[1], e[2]-other.e[2]}; }
    Vec3 operator*(double n) const { return Vec3{e[0]*n, e[1]*n, e[2]*n}; }
    Vec3 operator/(double n) const { return Vec3{e[0]/n, e[1]/n, e[2]/n}; }

    Vec3& operator+=(const Vec3& other) 
    { 
        this->e[0] += other.e[0];
        this->e[1] += other.e[1];
        this->e[2] += other.e[2];
        return *this;
    }

    Vec3& operator/=(double n) 
    { 
        this->e[0] /= n;
        this->e[1] /= n;
        this->e[2] /= n;
        return *this;
    }

    double Dot(const Vec3& other) const { return e[0]*other.e[0] + e[1]*other.e[1] + e[2]*other.e[2]; }
    Vec3 Cross(const Vec3& other) const 
    {
        return Vec3(
            e[1] * other.e[2] - e[2] * other.e[1],
            e[2] * other.e[0] - e[0] * other.e[2],
            e[0] * other.e[1] - e[1] * other.e[0]
        );
    }
    double Magnitude() const { return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    double MagnitudeSquared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
    Vec3 Normalized() const { return *this / this->Magnitude(); }

    bool IsNearZero() const 
    {
        double s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    Vec3 Reflect(const Vec3& normal)
    {
        return *this - normal*2*this->Dot(normal);
    }

    friend Vec3 operator*(double n, const Vec3& u);
    Vec3 Refract(const Vec3& n, double refraction_ratio)
    {
        double cos_theta = fmin((-*this).Dot(n), 1.0);
        Vec3 r_out_perp = refraction_ratio * (*this + cos_theta*n);
        Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.MagnitudeSquared())) * n;
        return r_out_perp + r_out_parallel; 
    }
    
    // Requieres the current object to be a unit vector
    Vec3 RandomInSameHemisphere() const 
    {
        Vec3 other = RandomUnitVector();
        return other.Dot(*this) > 0.0 ? other : -other;
    }

    // Simple random in-square generation is not enough, 
    // as the distribution in the square would be uniform, 
    // but not the distribution of the projected vectors on unit-sphere
    // Thus, we only take vector generated inside the sphere.
    static Vec3 RandomUnitVector() 
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

    static Vec3 Random() { return Vec3(random_double(), random_double(), random_double()); }
    static Vec3 Random(double min, double max) { return Vec3(random_double(min, max), random_double(min, max), random_double(min, max)); }
};


inline Vec3 operator*(double n, const Vec3& u) { return Vec3{n*u.e[0], n*u.e[1], n*u.e[2]}; }
inline Vec3 operator*(const Vec3& u, const Vec3& v) { return Vec3{u.e[0]*v.e[0], u.e[1]*v.e[1], u.e[2]*v.e[2]}; }


typedef Vec3 Point3;
typedef Vec3 RGBColor;