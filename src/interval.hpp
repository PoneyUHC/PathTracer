
#pragma once

#include <cmath>


class Interval {

private:

    double m_min;
    double m_max;


public:

    Interval(double min, double max) : m_min{min}, m_max{max} {}

    inline double Max() const { return m_max; }
    inline double Min() const { return m_min; }
    inline double Size() const { return m_max - m_min; }
    
    inline bool Contains(double x) const { return x >= m_min && x <= m_max; }
    inline bool Surrounds(double x) const { return x > m_min && x < m_max; }
    
    inline double Clamp(double x) const { x < m_min ? m_min : (x > m_max ? m_max : x); }

    static const Interval Empty, Universe;
};
