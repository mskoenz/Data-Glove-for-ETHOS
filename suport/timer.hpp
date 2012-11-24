// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.11.2012 12:15:09 CET
// File:    timer.hpp

#ifndef __TIMER_HEADER
#define __TIMER_HEADER

#include "Arduino.h"

//------------------- how to use -------------------
/* timer_class is a simple timer, that can be reset
 * the timer will overflow after approx 50 days
 * 
 * ctor:
 * timer_class timer; //time is set to zero
 * 
 * call:
 * timer();         //returns the milliseconds (unsigned long) since last reset
 * timer.reset();   //resets the timer
 */

class timer_class
{
public:
    timer_class(): ref_time(0)
    {}
    unsigned long operator()() const
    {
        return millis() - ref_time;
    }
    void reset()
    {
        ref_time = millis();
    }
private:
    unsigned long ref_time;
};


#endif //__TIMER_HEADER
