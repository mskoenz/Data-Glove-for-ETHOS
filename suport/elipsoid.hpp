// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    18.01.2013 16:28:08 CET
// File:    elipsoid.hpp

#ifndef __ELIPSOID_HEADER
#define __ELIPSOID_HEADER

#include "metric.hpp"

struct elipsoid_struct
{
    elipsoid_struct(point_struct const & o, point_struct const & r): origin(o), radius(r)
    {}
    bool contains(point_struct const & a)
    {
        std::cout << d(a,origin) - radius[4]*precision << std::endl;
        return true;
    }
    point_struct origin;
    point_struct radius;
};


#endif //__ELIPSOID_HEADER
