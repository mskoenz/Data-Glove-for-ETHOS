// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    04.05.2012 15:28:50 CEST
// File:    Blink.cpp


#include "Arduino.h"
#include "Wire.h"

typedef unsigned int uint;

#include "suport/frequency.hpp"
#include "suport/timer.hpp"
#include "suport/led.hpp"
#include "suport/sensor.hpp"
#include "suport/gestic.hpp"

//  +---------------------------------------------------+
//  |                   pin mapping                     |
//  +---------------------------------------------------+
#define GREEN_LED 10
#define YELLOW_LED 11
#define RED_LED 9
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
            , YELLOW_LED
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
    led.yellow();
    pinMode(BUTTON, INPUT);
    
    //------------------- serial -------------------
    Serial.begin(57600);
    gestic.write_to_eeprom();
    
    //------------------- I2C -------------------
    Wire.begin(GLOVE_ADRESS);
    Wire.onRequest(speak);
    Wire.onReceive(listen);
    
    //------------------- test EEPROM -------------------
    //~ int low0[] ={800,700,700,700,700,700,700,700,700};
    //~ int up0[] ={830,1000,1000,1000,1000,1000,1000,1000,1000};
    //~ int low1[] ={830,700,700,700,700,700,700,700,700};
    //~ int up1[] ={1000,1000,1000,1000,1000,1000,1000,1000,1000};
    //~ gestic.insert(low0, up0);
    //~ gestic.insert(low1, up1);
    //~ gestic.remove_all();
    led.green();
}
//  +---------------------------------------------------+
//  |                   main loop                       |
//  +---------------------------------------------------+
void loop()
{
    sensor.update(1);
    sensor.print();
    //~ gestic.update();
 
    //~ if(sensor.button())
    //~ {
        //~ learn_gesture();
        //~ gestic.print();
        //~ timer.reset();
    //~ }
    
    //~ if(gestic.get_current_gesture() != NO_GESTURE)
    //~ {
        //~ Serial.println(gestic.get_current_gesture());
        //~ if(gestic.get_current_gesture() == 0)
            //~ led.green();
        //~ if(gestic.get_current_gesture() == 1)
            //~ led.yellow();
        //~ if(gestic.get_current_gesture() == 2)
            //~ led.red();
    //~ }
    //~ else
        //~ led.none();
    
    //~ frequency(100);
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
    double val[9];
    double val2[9];
    
    for(uint i = 0; i < 9; ++i)
    {
        val[i] = 0;
        val2[i] = 0;
    }
    
    uint n = 5;
    for(uint i = 0; i < n; ++i)
    {
        Serial.print("  step: ");
        Serial.print(i);
        Serial.print(" ");
        led.yellow();
        delay(2000);
        led.red();
        sensor.update(100);
        while(sensor.velocity() > 4)
        {
            sensor.update(1000);
            Serial.print("  speed = ");
            Serial.println(sensor.velocity());
        }
        
        for(uint i = 0; i < 9; ++i)
        {
            val[i] += sensor[i];
            val2[i] += sensor[i]*double(sensor[i]);
        }
    }
    
    double avr;
    double var;
    int upper[9];
    int lower[9];
    
    for(uint i = 0; i < 9; ++i)
    {
         avr = val[i]/n;
         var = val2[i]/n - avr*avr;
         
         lower[i] = avr - max(var, 2);
         upper[i] = avr + max(var, 2);
    }
    
    Serial.println("new gesture:");
    for(uint i = 0; i < 9; ++i)
    {
        Serial.print(lower[i]);
        Serial.print(" <-> ");
        Serial.print(upper[i]);
        Serial.println();
    }
    led.yellow();
    
    gestic.insert(lower, upper);  //lower, upper
    
    led.green();
}
#include "main.cpp"
