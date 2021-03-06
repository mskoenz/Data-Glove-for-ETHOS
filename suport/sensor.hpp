// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    20.11.2012 23:42:39 CET
// File:    sensor.hpp

#ifndef __SENSOR_HEADER
#define __SENSOR_HEADER

#include "Arduino.h"
#include "accumulator.hpp"
//------------------- how to use -------------------
/* Construct via ctor, args are mux0, mux1, mux2, mux master, sensor8, button
 *      sensor_class sensor(...all pins...);
 * Call the update function in one of two ways:
 *      sensor.update();    //reads all sensors (9 analog-values take approx 1 ms)
 *      sensor.update(7);   //reads all sensors 7 (int) times and averages
 *      sensor[5];          //retruns the last measured or averaged value of sensor 5 as an int
 *      sensor.vel();       //retruns the velocity (more precise just the euclidean distance)
 *                          //(for now between last measured/averaded and current measured/averaged)
 */
const int cutoff = 650; //in order to get values between 0-255

typedef uint8_t sen_type;

class sensor_class
{
public:
    sensor_class( short mux0_pin
                , short mux1_pin
                , short mux2_pin
                , short mux_master_pin
                , short sensor8_pin
                , short button_pin
                ):mux0_pin_(mux0_pin)
                , mux1_pin_(mux1_pin)
                , mux2_pin_(mux2_pin)
                , mux_master_pin_(mux_master_pin)
                , sensor8_pin_(sensor8_pin)
                , button_pin_(button_pin)
                , sensor_()
                , sensor_old_()
                , button_(false)
                , vel2_(0)
    {
        //these are the pins to set the mux
        pinMode(mux0_pin_, OUTPUT);
        pinMode(mux1_pin_, OUTPUT);
        pinMode(mux2_pin_, OUTPUT);
        pinMode(button_pin_, INPUT);
        //analog pins are only input, therefor no pinMode
    }
    //called in the loop, reads all sensor_s
    void update()
    {
        update_old();
        //looks silly, but fastes way (known to me) to read, since only one digital_write per analog_read
        digitalWrite(mux0_pin_, LOW);
        digitalWrite(mux1_pin_, LOW);
        digitalWrite(mux2_pin_, LOW);
        sensor_[0] = analogRead(mux_master_pin_) - cutoff;
        
        digitalWrite(mux0_pin_, HIGH);
        sensor_[1] = analogRead(mux_master_pin_) - cutoff;
        
        digitalWrite(mux1_pin_, HIGH);
        sensor_[3] = analogRead(mux_master_pin_) - cutoff;

        digitalWrite(mux0_pin_, LOW);
        sensor_[2] = analogRead(mux_master_pin_) - cutoff;
        
        digitalWrite(mux2_pin_, HIGH);
        sensor_[6] = analogRead(mux_master_pin_) - cutoff;
        
        digitalWrite(mux0_pin_, HIGH);
        sensor_[7] = analogRead(mux_master_pin_) - cutoff;
        
        digitalWrite(mux1_pin_, LOW);
        sensor_[5] = analogRead(mux_master_pin_) - cutoff;
        
        digitalWrite(mux0_pin_, LOW);
        sensor_[4] = analogRead(mux_master_pin_) - cutoff;
        
        sensor_[8] = analogRead(sensor8_pin_) - cutoff;
        
        //read button
        button_ = digitalRead(button_pin_);
        
        //calculate velocity^2
        vel2_ = 0;
        for(int i = 0; i < 9; ++i)
        {
            vel2_ += (sensor_old_[i] - sensor_[i]) * 1.0 * (sensor_old_[i] - sensor_[i]);
        }
    }
    int velocity() const
    {
        return sqrt(vel2_);
    }
    //updates, but takes the average over t sensor-measurements
    void update(int const t)
    {
        accumulator_interval_class acc[9];
        //average-loop
        for(int i = 0; i < t; ++i)
        {
            digitalWrite(mux0_pin_, LOW);
            digitalWrite(mux1_pin_, LOW);
            digitalWrite(mux2_pin_, LOW);
            acc[0] << analogRead(mux_master_pin_) - cutoff;
            
            digitalWrite(mux0_pin_, HIGH);
            acc[1] << analogRead(mux_master_pin_) - cutoff;
            
            digitalWrite(mux1_pin_, HIGH);
            acc[3] << analogRead(mux_master_pin_) - cutoff;

            digitalWrite(mux0_pin_, LOW);
            acc[2] << analogRead(mux_master_pin_) - cutoff;
            
            digitalWrite(mux2_pin_, HIGH);
            acc[6] << analogRead(mux_master_pin_) - cutoff;
            
            digitalWrite(mux0_pin_, HIGH);
            acc[7] << analogRead(mux_master_pin_) - cutoff;
            
            digitalWrite(mux1_pin_, LOW);
            acc[5] << analogRead(mux_master_pin_) - cutoff;
            
            digitalWrite(mux0_pin_, LOW);
            acc[4] << analogRead(mux_master_pin_) - cutoff;
            
            acc[8] << analogRead(sensor8_pin_) - cutoff;
        }
        
        
        update_old();
        //write mean to sensor_
        for(int i = 0; i < 9; ++i)
        {
            sensor_[i] = acc[i].mean();
        }
        //read button
        button_ = digitalRead(button_pin_);
        
        //calculate velocity^2
        vel2_ = 0;
        for(uint8_t i = 0; i < 9; ++i)
        { //idea: add sensor_old to accumulator and calc diagonal of the hyperrectangle that contains all points in acc 
            acc[i] << sensor_old_[i];
            vel2_ += (acc[i].diff()) * 1.0 * (acc[i].diff());
        }
    }
    sen_type const operator[](int const i) const
    {
        return sensor_[i];
    }
    bool const button() const
    {
        return button_;
    }
    void print() const
    {
        for(uint8_t i = 0; i < 9; ++i)
        {
            Serial.print(sensor_[i]);
            Serial.print(" ");
        }
        Serial.println(button_);
    }
private:
    //update old writes the sensor_ to sensor_old_
    inline void update_old()
    {
        for(int i = 0; i < 9; ++i)
        {
            sensor_old_[i] = sensor_[i];
        }
    }
    //=================== members ===================
    uint8_t const mux0_pin_;
    uint8_t const mux1_pin_;
    uint8_t const mux2_pin_;
    uint8_t const mux_master_pin_;
    uint8_t const sensor8_pin_;
    uint8_t const button_pin_;
    
    sen_type sensor_[9];
    sen_type sensor_old_[9];
    bool button_;
    float vel2_;
};
#endif //__SENSOR_HEADER
