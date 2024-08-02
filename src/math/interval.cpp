
#include "math/interval.hpp"

#include <cmath>


const Interval Interval::Empty = Interval(INFINITY, -INFINITY);
const Interval Interval::Universe = Interval(-INFINITY, INFINITY);


bool Interval::Overlaps(const Interval &other) const
{
    return std::fmax(m_min, other.Min() < std::fmin(m_max, other.Max()));
}
