// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.11.2012 00:22:22 CET
// File:    accumulator.hpp

#ifndef __ACCUMULATOR_HEADER
#define __ACCUMULATOR_HEADER

//------------------- how to use -------------------
/* Construct via default ctor, no args needed:
 *      accumulator_class acc;
 * stream values (castable to double)into the accumulator
 *      acc << 9;
 * after some calls, one can get mean, error and variance like this:
 *      acc.mean(); //returns a double
 *      acc.error(); //returns a double
 *      acc.variance(); //returns a double
 * 
 * 
 */
class accumulator_class
{
public:
    accumulator_class(): count_(0), sum_(0), sum2_(0)
    {}
    void operator<<(double const val)
    {
        ++count_;
        sum_ += val;
        sum2_ += val * val;
    }
    double const mean() const
    {
        return sum_ / count_;
    }
    double const variance() const
    {
        return (sum2_ / count_ - mean() * mean());
    }
    double const error() const
    {
        return sqrt(variance());
    }
protected:
    double count_;
    double sum_;
    double sum2_;
};


#endif //__ACCUMULATOR_HEADER
