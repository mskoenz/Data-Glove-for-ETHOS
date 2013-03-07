// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    04.05.2012 15:28:50 CEST
// File:    Blink.cpp


#include "Arduino.h"
#include "Wire.h"
#include "freeRAM.hpp"

#include "suport/frequency.hpp"
#include "suport/accumulator_interval.hpp"
#include "suport/timer.hpp"
#include "suport/led.hpp"
#include "suport/sensor.hpp"
#include "suport/gestic.hpp"

//  +---------------------------------------------------+
//  |                   pin mapping                     |
//  +---------------------------------------------------+
#define BLUE_LED 10
#define GREEN_LED 9
#define RED_LED 11
#define BUTTON 5

//mux-pins
#define MUX0_PIN 2
#define MUX1_PIN 3
#define MUX2_PIN 4
#define MUX_MASTER_PIN A0

//because i have 9 sensors, a 8:1 MUX isn't enough. !! i starts counting at 0 !!
#define SENSOR8_PIN A1

//  +---------------------------------------------------+
//  |                   declare all functions           |
//  +---------------------------------------------------+
void speak();
void listen(int const n_byte);
void reset_atmega();
void learn_gesture();

//  +---------------------------------------------------+
//  |                   define global variables         |
//  +---------------------------------------------------+
frequency_class frequency;

timer_class timer;

led_class led(RED_LED
            , BLUE_LED
            , GREEN_LED);

#include "suport/i2c.hpp"
sensor_class sensor(  MUX0_PIN
                    , MUX1_PIN
                    , MUX2_PIN
                    , MUX_MASTER_PIN
                    , SENSOR8_PIN
                    , BUTTON);

gestic_class gestic(sensor, timer); //const ref on sensor & timer

i2c_class i2c(gestic, sensor, timer); //const ref on sensor but normal ref on gestic & timer

//  +---------------------------------------------------+
//  |                   setup                           |
//  +---------------------------------------------------+
void setup()
{
    //------------------- pin modes -------------------
    led.blue();
    pinMode(BUTTON, INPUT);
    //~ Serial.begin(57600);
    
    //------------------- I2C -------------------
    Wire.begin(GLOVE_ADRESS);
    Wire.onRequest(speak);
    Wire.onReceive(listen);
    
    led.green();
}
//  +---------------------------------------------------+
//  |                   main loop                       |
//  +---------------------------------------------------+
void loop()
{
    sensor.update();
    
    gestic.update();
    
    if(sensor.button())
    {
        learn_gesture();
    }
    
    if(gestic.get_current_gesture() != NO_GESTURE)
    {
        if(gestic.get_current_gesture()%2 == 0)
            led.red();
        if(gestic.get_current_gesture()%2 == 1)
            led.blue();
    }
    else
        led.green();
    
    //~ frequency(200);
}
//  +---------------------------------------------------+
//  |                   I2C functions                   |
//  +---------------------------------------------------+
void speak()
{
    i2c.speak();
}

void listen(int const n_byte)
{
    i2c.listen(n_byte);
}
//  +---------------------------------------------------+
//  |                   reset                           |
//  +---------------------------------------------------+
void reset_atmega()
{
    asm volatile ("  jmp 0"); //not best method, but ok if all is initialize correct
}

//  +---------------------------------------------------+
//  |                   functions                       |
//  +---------------------------------------------------+
inline void learn_gesture()
{
    accumulator_interval_class val[9];
    accumulator_interval_class val2[9];
    
    //~ for(uint8_t i = 0; i < 9; ++i)
    //~ {
        //~ val[i] = 0;
        //~ val2[i] = 0;
    //~ }
    
    uint8_t n = 5;
    for(uint8_t i = 0; i < n; ++i)
    {
        led.blue();
        for(int i = 0; i < 20; ++i)
        {
            delay(100);
        }
        
        led.red();
        sensor.update(300);
        while(sensor.velocity() > 40) //while sensor is realy fast -> wait
        {
            Serial.println(sensor.velocity());
            sensor.update(300);
        }
        for(uint8_t i = 0; i < 9; ++i) //register point before the sensor hysteresis happens
            val[i] << sensor[i];

        while(sensor.velocity() > 5) //wait until the sensor is relaxed
        {
            //~ Serial.println(sensor.velocity());
            sensor.update(300);
        }
        for(uint8_t i = 0; i < 9; ++i) //register end point
            val2[i] << sensor[i];
    }
    
    //~ double avr;
    //~ double var;
    uint8_t upper[9];
    uint8_t lower[9];
    
    for(uint8_t i = 0; i < 9; ++i)
    {
        lower[i] = min(val[i].get_min(), val2[i].get_min());
        upper[i] = max(val[i].get_max(), val2[i].get_max());
    }
    
    led.blue();
    
    gestic.insert(lower, upper);  //lower, upper
        
    led.green();
}
#include "main.cpp"
