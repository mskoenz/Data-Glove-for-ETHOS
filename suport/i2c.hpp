// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.11.2012 13:04:34 CET
// File:    i2c.hpp

#ifndef __I2C_HEADER
#define __I2C_HEADER

#include "Arduino.h"
#include "i2c_enum.h"
#include "sensor.hpp"
#include "timer.hpp"
#include "gestic.hpp"
#include "i2c_io.hpp"

class i2c_class
{
public:
    i2c_class(gestic_class & gestic         //not const!
            , sensor_class const & sensor
            , timer_class & timer           //not const!
            ): gest_ref_(gestic)
             , sen_cref_(sensor)
             , t_ref_(timer)
             , request_(0)
             , rec_(false)
    {
    }
    inline void listen(int const n_byte)
    {
        request_ = 0;
        
        for(int i = 0; i < n_byte; ++i)
        {
            request_ = Wire.read();
        }
        
        switch(request_)
        {
            case(get_num_of_gestures):
                //nothing to do
                break;
            case(get_current_gesture):
                //nothing to do
                break;
            case(get_raw_values):
                //nothing to do
                break;
            case(get_last_gestures):
                for(uint8_t i = 0; i < 8; ++i) //save in temp, for faster sending
                    temp[i] = (gest_ref_.get_last_eight_gestures()[i]);
                break;
            case(record_gesture):
                led.red();
                rec_ = true;
                break;
            case(record_progress):
                break;
            case(stop_record):
                led.green();
                rec_ = false;
                break;
            case(reset_time):
                t_ref_.reset();
                break;
            case(delete_all_gestures):
                gest_ref_.remove_all();
                break;
            case(write_gestures_to_eeprom):
                gest_ref_.write_to_eeprom();
                break;
            case(reset_glove):
                reset_atmega();
                break;
            case(get_all_gestures):
                //nothing to do
                break;
            default:
                break;
        }
    }
    inline void speak() const
    {
        switch(request_)
        {
            case(get_num_of_gestures):
                i2cout << gest_ref_.get_n_gesture();
                i2cout.str();
                break;
            case(get_current_gesture):
                i2cout << gest_ref_.get_current_gesture();
                i2cout << t_ref_();
                i2cout.str();
                break;
            case(get_raw_values):
                for(uint8_t i = 0; i < 9; ++i)
                {
                    i2cout << sen_cref_[i];
                }
                i2cout.str();
                break;
            case(get_last_gestures):
                for(uint8_t i = 0; i < 8; ++i)
                {
                    i2cout << temp[i].gesture;
                    i2cout << temp[i].time;
                }
                i2cout.str();
                break;
            case(record_gesture):
                //nothing to tell
                break;
            case(record_progress):
                //nothing to tell
                break;
            case(stop_record):
                //nothing to tell
                break;
            case(reset_time):
                //nothing to tell
                break;
            case(delete_all_gestures):
                //nothing to tell
                break;
            case(write_gestures_to_eeprom):
                //nothing to tell
                break;
            case(reset_glove):
                //nothing to tell
                break;
            case(get_all_gestures):
                for(uint8_t i = 0; i < 9; ++i)
                {
                    uint8_t n = gest_ref_.get_n_gesture();
                    if(n == 0)
                    {
                        i2cout << uint8_t(1);
                        i2cout << uint8_t(1);
                    }
                    else
                    {
                        i2cout << gest_ref_(n - 1, low_bound, i);
                        i2cout << gest_ref_(n - 1, high_bound, i);
                    }
                }
                i2cout.str();
                break;
            default:
                break;
        }
    }
    inline bool rec() const
    {
        return rec_;
    }
private:
    gestic_class & gest_ref_;
    sensor_class const & sen_cref_;
    timer_class & t_ref_;
    
    int request_;   //what does the master want (i2c_glove_enum)
    bool rec_;      //rec_ is on, if one wants to record a new gesture
    
    gesture_with_time_struct temp[8]; //to save time during sending
};


#endif //__I2C_HEADER
