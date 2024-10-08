
#include "aabb.hpp"

#include "math/ray.hpp"

#include <cmath>


using namespace std;


const AABB AABB::Empty = AABB(Interval::Empty, Interval::Empty, Interval::Empty); 
const AABB AABB::Universe = AABB(Interval::Universe, Interval::Universe, Interval::Universe); 


const Interval& AABB::IndexToInterval(int index) const
{
    if (index == 0) return m_x;
    if (index == 1) return m_y;
    return m_z;
}


int AABB::LongestAxis() const
{
    if (m_x.Size() > m_y.Size())
        return m_x.Size() > m_z.Size() ? 0 : 2;
    else
        return m_y.Size() > m_z.Size() ? 1 : 2;
}


AABB::AABB(const Interval &x, const Interval &y, const Interval &z)
    : m_x{x}, m_y{y}, m_z{z}
{
    PadToMinimums();
}


AABB::AABB(const Point3 &a, const Point3 &b)
{
    m_x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
    m_y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
    m_z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);

    PadToMinimums();
}


AABB::AABB(const AABB &aabb1, const AABB &aabb2)
{
    m_x = Interval(aabb1.m_x, aabb2.m_x);
    m_y = Interval(aabb1.m_y, aabb2.m_y);
    m_z = Interval(aabb1.m_z, aabb2.m_z);
}


bool AABB::Hit(const Ray &ray, const Interval &interval) const
{
    const Point3& ray_origin = ray.Origin();
    const Vec3& ray_direction = ray.Direction();

    Interval overlap = interval;

    for(int i=0; i<3; ++i){
        const Interval& axis_interval = IndexToInterval(i);
        const double dir_inv = 1.0 / ray_direction.e[i];

        double t0 = (axis_interval.Min() - ray_origin.e[i]) * dir_inv;
        double t1 = (axis_interval.Max() - ray_origin.e[i]) * dir_inv;

        if (t0 < t1){
            if (t0 > overlap.Min()) overlap.SetMin(t0);
            if (t1 < overlap.Max()) overlap.SetMax(t1);
        } else {
            if (t1 > overlap.Min()) overlap.SetMin(t1);
            if (t0 < overlap.Max()) overlap.SetMax(t0);
        }

        if (overlap.Max() <= overlap.Min()){
            return false;
        }
    }

    return true;
}


void AABB::PadToMinimums()
{
    static double delta = 0.0001;
    if(m_x.Size() < delta) m_x.Expand(delta);
    if(m_y.Size() < delta) m_y.Expand(delta);
    if(m_z.Size() < delta) m_z.Expand(delta);
}
