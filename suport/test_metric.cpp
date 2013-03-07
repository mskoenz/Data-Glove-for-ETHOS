// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    18.01.2013 15:59:41 CET
// File:    test_metric.cpp

#include <iostream>
#include "elipsoid.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    point_struct a;
    point_struct b;
    point_struct c;
    for(int i = 0; i < 9; ++i)
    {
        a[i] = i;
        b[i] = 5;
        c[i] = 3+i/2;
    }
    elipsoid_struct e(a,b);
    
    std::cout << d(a,b) << std::endl;
    
    e.contains(c);
    return 0;
}
