// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    18.11.2012 17:33:11 CET
// File:    frequency.hpp

#ifndef __FREQUENCY_HEADER
#define __FREQUENCY_HEADER

#include "Arduino.h"
//------------------- how to use -------------------
/* Construct via default ctor: 
 *      frequency_class freq();
 * Call the operator() in the main loop. k defines, how often the output is shown (big k == low overhead)
 *      freq(200); //Shows the frequency every 200st loop. Needs a serial connection
 */

//------------------- class for checking loop-frequency -------------------
class frequency_class
{
public:
    frequency_class(): index(0), millis_old(0), millis_now(0)
    {}
    void operator()(uint const k)
    {
        if(index % k == 0)
        {
            millis_old = millis_now;
            millis_now = millis();
            Serial.print("Frequency: ");
            Serial.println(k / (millis_now - millis_old) * 1000);
        }
    ++index;
    }
    
private:
    long unsigned index;
    double millis_old;
    double millis_now;
};
#endif //__FREQUENCY_HEADER
