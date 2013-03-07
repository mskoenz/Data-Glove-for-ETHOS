// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    18.01.2013 15:50:43 CET
// File:    metric.hpp

#ifndef __METRIC_HEADER
#define __METRIC_HEADER

#include <iostream>
#include <fstream>
#include <cmath>

/// +---------------------------------------------------+
/// |                   constants                       |
/// +---------------------------------------------------+
const int precision = 100; //multiplies sqrt(x) with precision before cast to int

//  +---------------------------------------------------+
//  |                   point                           |
//  +---------------------------------------------------+
struct point_struct
{
    int & operator[](const int i)
    {
        return p[i];
    }
    const int operator[](const int i) const
    {
        return p[i];
    }
    int p[9];
};
std::ostream & operator<<(std::ostream & os, point_struct const & a)
{
    os << "(";
    for(int i = 0; i < 8; ++i)
    {
        os << a[i] << ", ";
    }
    os << a[8] << ")" << std::endl;
    return os;
}
point_struct operator+(point_struct const & a, point_struct const & b)
{
    point_struct c;
    for(int i = 0; i < 9; ++i)
    {
        c[i] = a[i] + b[i];
    }
    return c;
}
point_struct operator-(point_struct const & a, point_struct const & b)
{
    point_struct c;
    for(int i = 0; i < 9; ++i)
    {
        c[i] = a[i] - b[i];
    }
    return c;
}

int d(point_struct const & a, point_struct const & b)
{
    int res = 0;
    for(int i = 0; i < 9; ++i)
    {
        res += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return precision * sqrt(res);
}

int d(int const a[9], int const b[9])
{
    int res = 0;
    for(int i = 0; i < 9; ++i)
    {
        res += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return precision*sqrt(res);
}

#endif //__METRIC_HEADER
