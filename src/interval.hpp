
#pragma once


class Interval {

private:

    double m_min;
    double m_max;


public:

    Interval(double min, double max) : m_min{min}, m_max{max} {}

    double Max() const { return m_max; }
    double Min() const { return m_min; }
    double Size() const { return m_max - m_min; }
    
    bool Contains(double x) const { return x >= m_min && x <= m_max; }
    bool Surrounds(double x) const { return x > m_min && x < m_max; }
    
    double Clamp(double x) const { return x < m_min ? m_min : (x > m_max ? m_max : x); }

    static const Interval Empty, Universe;
};
