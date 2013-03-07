// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    18.11.2012 17:45:11 CET
// File:    led.hpp

#ifndef __LED_HEADER
#define __LED_HEADER

#include "Arduino.h"
//------------------- how to use -------------------
/* Construct with ctor, that takes the pin-numbers of the colors red, blue, green
 *      led_class led(pin_red, pin_blue, pin_green)
 * Use the member-fct red, blue & green to change the color
 *      led.green();    //sets pins red & blue HIGH and green LOW
 *      led.blue();   //sets pins green & red HIGH and blue LOW
 *      led.red();      //sets pins green & blue HIGH and red LOW
 *      led.none();      //sets pins green & blue and red HIGH
 * HIGH means no current, since common cathode
 */

//------------------- led class for 3 color led -------------------
class led_class
{
public:
    led_class(short const red, short const blue, short const green):
        red_(red), blue_(blue), green_(green) 
    {
        pinMode(red_, OUTPUT);
        pinMode(green_, OUTPUT);
        pinMode(blue_, OUTPUT);
    }
    void red() const
    {
        digitalWrite(green_, HIGH);
        digitalWrite(blue_, HIGH);
        digitalWrite(red_, LOW);
    }
    void blue() const
    {
        digitalWrite(green_, HIGH);
        digitalWrite(blue_, LOW);
        digitalWrite(red_, HIGH);
    }
    void green() const
    {
        digitalWrite(green_, LOW);
        digitalWrite(blue_, HIGH);
        digitalWrite(red_, HIGH);
    }
    void none() const
    {
        digitalWrite(green_, HIGH);
        digitalWrite(blue_, HIGH);
        digitalWrite(red_, HIGH);
    }
private:
    short red_;
    short blue_;
    short green_;
};

#endif //__LED_HEADER
