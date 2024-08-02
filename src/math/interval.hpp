
#pragma once


class Interval {

private:

    double m_min;
    double m_max;


public:

    Interval() {}
    Interval(double min, double max): m_min{min}, m_max{max} {}
    Interval(const Interval& i1, const Interval& i2) {
        m_min = i1.m_min <= i2.m_min ? i1.m_min : i2.m_min;
        m_max = i1.m_max >= i2.m_max ? i1.m_max : i2.m_max;
    }

    double Max() const { return m_max; }
    double SetMax(double value) { return m_max = value; }
    double Min() const { return m_min; }
    double SetMin(double value) { return m_min = value; }
    double Size() const { return m_max - m_min; }
    
    bool Contains(double x) const { return x >= m_min && x <= m_max; }
    bool Surrounds(double x) const { return x > m_min && x < m_max; }
    bool Overlaps(const Interval& other) const;
    
    void Expand(double delta) 
    { 
        double padding = delta / 2;
        m_min -= padding;
        m_max += padding;
    }
    
    double Clamp(double x) const { return x < m_min ? m_min : (x > m_max ? m_max : x); }

    static const Interval Empty, Universe;
};
