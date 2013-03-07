// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    07.03.2013 16:16:59 CET
// File:    accumulator_interval.hpp

#ifndef __ACCUMULATOR_INTERVAL_HEADER
#define __ACCUMULATOR_INTERVAL_HEADER

#include "accumulator.hpp"
//------------------- how to use -------------------
/* Construct via default ctor, no args needed:
 *      accumulator_interval_class acc;
 * stream values (castable to double)into the accumulator
 *      acc << 9;
 * after some calls, one can get mean, error and variance like this:
 *      acc.mean(); //returns a double
 *      acc.error(); //returns a double
 *      acc.min(); //returns a double
 *      acc.max(); //returns a double
 *      acc.variance(); //returns a double
 * 
 * 
 */
class accumulator_interval_class: public accumulator_class
{
    typedef accumulator_class base_type;
public:
    accumulator_interval_class(): base_type(), min_(0), max_(0)
    {}
    void operator<<(double const val)
    {
        base_type::operator<<(val);
        if(count_ != 1)
        {
            if(val < min_)
                min_ = val;
            if(val > max_)
                max_ = val;
        }
        else //first accsess
        {
            min_ = val;
            max_ = val;
        }
    }
    double const get_min() const
    {
        return min_;
    }
    double const get_max() const
    {
        return max_;
    }
    double const diff() const
    {
        return max_ - min_;
    }
protected:
    double min_;
    double max_;
};

#endif //__ACCUMULATOR_INTERVAL_HEADER
