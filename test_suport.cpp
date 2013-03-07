// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    07.03.2013 16:21:26 CET
// File:    main.cpp

#include <iostream>
#include <cmath>
#include "suport/accumulator_interval.hpp"


using namespace std;

#define pr(x) cout << #x << ": " << ai.x() << endl;

int main(int argc, char* argv[])
{
    accumulator_interval_class ai;
    
    ai << 1;
    ai << 3;
    ai << 3;
    ai << 2;
    ai << 2;
    ai << -2;
    ai << 8;
    ai << 9;
    
    pr(mean)
    pr(error)
    pr(variance)
    pr(get_min)
    pr(get_max)
    
    return 0;
}
