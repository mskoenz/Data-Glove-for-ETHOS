// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    18.11.2012 17:45:11 CET
// File:    led.hpp

#ifndef __LED_HEADER
#define __LED_HEADER

#include "Arduino.h"
//------------------- how to use -------------------
/* Construct with ctor, that takes the pin-numbers of the colors red, yellow, green
 *      led_class led(pin_red, pin_yellow, pin_green)
 * Use the member-fct red, yellow & green to change the color
 *      led.green(); //sets pins red & yellow LOW and green HIGH
 */

//------------------- led class for 3 color led -------------------
class led_class
{
public:
    led_class(short const red, short const yellow, short const green):
        red_(red), yellow_(yellow), green_(green) 
    {
        pinMode(red_, OUTPUT);
        pinMode(green_, OUTPUT);
        pinMode(yellow_, OUTPUT);
    }
    void red() const
    {
        digitalWrite(green_, LOW);
        digitalWrite(yellow_, LOW);
        digitalWrite(red_, HIGH);
    }
    void yellow() const
    {
        digitalWrite(green_, LOW);
        digitalWrite(yellow_, HIGH);
        digitalWrite(red_, LOW);
    }
    void green() const
    {
        digitalWrite(green_, HIGH);
        digitalWrite(yellow_, LOW);
        digitalWrite(red_, LOW);
    }
    void none() const
    {
        digitalWrite(green_, LOW);
        digitalWrite(yellow_, LOW);
        digitalWrite(red_, LOW);
    }
private:
    short red_;
    short yellow_;
    short green_;
};

#endif //__LED_HEADER
