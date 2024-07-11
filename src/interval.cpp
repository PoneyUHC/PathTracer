
#include "interval.hpp"

#include <cmath>


const Interval Interval::Empty = Interval(INFINITY, -INFINITY);
const Interval Interval::Universe = Interval(-INFINITY, INFINITY);