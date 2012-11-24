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
#define GREEN_LED 2
#define YELLOW_LED 3
#define RED_LED 4
#define BUTTON 5

//mux-pins
#define MUX0_PIN 11
#define MUX1_PIN 10
#define MUX2_PIN 9
#define MUX_MASTER_PIN A0

//because i have 9 sensors, a 8:1 MUX isn't enough. !! i start counting at 0 !!
#define SENSOR8_PIN A1

//  +---------------------------------------------------+
//  |                   declare all functions           |
//  +---------------------------------------------------+
void speak();
void listen(int const n_byte);
void reset_atmega();

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
    int low[] ={1,1,1,1,1,1,1,1,1};
    int up[] ={1000,1000,1000,1000,1000,1000,1000,1000,1000};
    gestic.insert(low, up);
    
    led.green();
}
//  +---------------------------------------------------+
//  |                   main loop                       |
//  +---------------------------------------------------+
void loop()
{
    sensor.update();
    
    if(sensor.button())
    {
        gestic.update();
    }
    if(sensor[8] > 1000)
    {
        gestic.print();
        timer.reset();
    }
    
    frequency(100);
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
//~ inline void record_gesture()
//~ {
    //~ double val[] = {0, 0};
    //~ double val2[] = {0, 0};
    //~ uint n = 5;
    //~ for(uint i = 0; i < n; ++i)
    //~ {
        //~ led.yellow();
        //~ delay(2000);
        //~ led.red();
        //~ sen_now[0] = 0;
        //~ sen_now[1] = 0;
        //~ while(speed() > 1)
        //~ {
            //~ measure(1000);
            //~ Serial.print("  speed = ");
            //~ Serial.println(speed());
        //~ }
        //~ Serial.print("sen[0] = ");
        //~ Serial.print(sen_now[0]);
        //~ Serial.print("  sen[1] = ");
        //~ Serial.print(sen_now[1]);
        //~ val[0] += sen_now[0];
        //~ val[1] += sen_now[1];
        //~ 
        //~ val2[0] += sen_now[0]*double(sen_now[0]);
        //~ val2[1] += sen_now[1]*double(sen_now[1]);
    //~ }
    //~ 
    //~ double avr[] = {val[0]/n, val[1]/n};
    //~ double var[] = {val2[0]/n - avr[0]*avr[0], val2[0]/n - avr[0]*avr[0]};
    //~ 
    //~ Serial.println("new gesture: sen0 = ");
    //~ Serial.print(uint(avr[0]));
    //~ Serial.print(" +/- ");
    //~ Serial.print(max(var[0], 2));
    //~ Serial.print(" sen1 = ");
    //~ Serial.print(uint(avr[1]));
    //~ Serial.print(" +/- ");
    //~ Serial.println(max(var[1], 2));
    //~ 
    //~ led.yellow();
    //~ 
    //~ int upper[] = {avr[0] + max(var[0], 2), avr[1] + max(var[1], 2)};
    //~ int lower[] = {avr[0] - max(var[0], 2), avr[1] - max(var[1], 2)};
    //~ 
    //~ Serial.print(upper[0]);
    //~ Serial.print(" ");
    //~ Serial.print(upper[1]);
    //~ Serial.print(" ");
    //~ Serial.print(lower[0]);
    //~ Serial.print(" ");
    //~ Serial.print(lower[1]);
    //~ Serial.print(" ");
    //~ 
    //~ vTree.insert(lower, upper);
    //~ vTree.build_tree();
    //~ 
    //~ led.green();
//~ }
#include "main.cpp"
