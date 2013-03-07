// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.11.2012 00:43:56 CET
// File:    gestic.hpp

#ifndef __GESTIC_HEADER
#define __GESTIC_HEADER

#include "Arduino.h"
#include "sensor.hpp"
#include "timer.hpp"

#define MAX_GESTURES 20
enum bound_descriptor
{
      low_bound = 0
    , high_bound = 1
    , BONDS = 2
    , NO_GESTURE = 255
};

#include "eeprom_manager.hpp"

struct gesture_with_time_struct
{
    uint8_t gesture;
    unsigned long time;
};

class gestic_class
{
public:
    gestic_class(  sensor_class const & sensor
                 , timer_class const & timer): sen_cref_(sensor)
                                        , t_cref_(timer)
                                        , EEPROM_manager()
                                        , barrel_(0)
                                        , hist_update_(0)
                                        , active_(false)
                                        , n_gesture_(0)
    {
        //------------------- set all to zero -------------------
        for(int i = 0; i < 8; ++i)
        {
            revolver_[i].gesture = NO_GESTURE;
            revolver_[i].time = 0;
            history_[i].gesture = NO_GESTURE;
            history_[i].time = 0;
        }

        for(int i = 0; i < MAX_GESTURES; ++i)
            for(int j = 0; j < BONDS; ++j)
                for(int k = 0; k < 9; ++k)
                    gesture_[i][j][k] = 0;
        
        //------------------- read from EEPROM -------------------
        EEPROM_manager.read(n_gesture_, gesture_);
        
    }
    //check, if sensor-value lies in a gesture
    void update()
    {
        int res = query();
        //~ static int res = 1;
        //~ res++;
        
        if(res != NO_GESTURE) //means, that a gesture is recognized
        {
            if(res != revolver_[barrel_ & 7].gesture or !active_) //a new gesture is recorded
            {
                ++barrel_;
                revolver_[barrel_ & 7].gesture = res; //&7 is a bitmask. barrel_%8 has the same effect, but slower
                revolver_[barrel_ & 7].time = t_cref_();
            }
            active_ = true;
        }
        else
            if(t_cref_() - revolver_[barrel_ & 7].time > 400) //deprelling
                active_ = false; //no gesture is recognized
    }
    //returns the amount of gestures as an int
    uint8_t get_n_gesture() const
    {
        return n_gesture_;
    }
    //returns the number of the active gesture, or -1 if no gesture is active
    uint8_t get_current_gesture() const
    {
        if(active_)
            return revolver_[barrel_ & 7].gesture; //& 7 is equivalent to % 8 but faster
        else
            return NO_GESTURE;
    }
    //retruns an array of the last 8 gestures, newest to oldest
    gesture_with_time_struct* get_last_eight_gestures()
    {
        if(barrel_ != hist_update_)     //if there was a change in revolver_, create history_
        {
            hist_update_ = barrel_;     //hist_update_ knows, how up to date history_ is
            for(int i = 0; i < 8; ++i)
            {
                history_[i] = revolver_[(barrel_ + 8 - i) & 7]; //reverse order and start at right index
            }
        }
        return history_;
    }
    //insert new intervall
    void insert(uint8_t lower[9], uint8_t upper[9])
    {
        if(n_gesture_ < MAX_GESTURES) //protect from overflow
        {
            for(int i = 0; i < 9; ++i)
            {
                gesture_[n_gesture_][low_bound][i] = lower[i];
                gesture_[n_gesture_][high_bound][i] = upper[i];
            }
            ++n_gesture_; //increment in the end, bc start counting at zero
        }
    }
    //remove all gestures
    void remove_all()
    {
        for(int i = 0; i < n_gesture_; ++i)
            for(int j = 0; j < BONDS; ++j)
                for(int k = 0; k < 9; ++k)
                    gesture_[i][j][k] = 0;
        
        n_gesture_ = 0;
        EEPROM_manager.remove_all(); //rewrites only n_gestures
    }
    void write_to_eeprom() const
    {
        EEPROM_manager.write(n_gesture_, gesture_);
    }
    //primitive approach, but ok, since MAX_GESTURES <=30
    uint8_t query() const
    {
        int right;
        for(int i = 0; i < n_gesture_; ++i)
        {
            right = 0;
            for(int k = 0; k < 9; ++k)
            {
                if(sen_cref_[k] <= gesture_[i][high_bound][k] and sen_cref_[k] >= gesture_[i][low_bound][k])
                    ++right;
                else
                    break;
            }
            
            if(right == 9)
                return i;
        }
        return NO_GESTURE;
    }
    //------------------- print -------------------
    void print() const
    {
        Serial.print("Revolver: ");
        for(uint8_t i = 0; i < 8; ++i)
        {
            Serial.print(revolver_[i].gesture);
            Serial.print(" (");
            Serial.print(revolver_[i].time);
            Serial.print(")  ");
        }
        Serial.println(" ");
        
        Serial.println("Gestic: ");
        for(int i = 0; i < n_gesture_; ++i)
        {
            for(uint8_t j = 0; j < BONDS; ++j)
            {
                for(uint8_t k = 0; k < 9; ++k)
                {
                    Serial.print(gesture_[i][j][k]);
                    Serial.print(" ");
                }
                Serial.print("     ");
            }
            Serial.println();
        }
        Serial.println("Sensor: ");
        sen_cref_.print();
    }
    uint8_t const operator()(int const & n, bound_descriptor const & b, int const & dim)
    {
        return gesture_[n][b][dim];
    }
private:
    sensor_class const & sen_cref_;
    timer_class const & t_cref_;
    EEPROM_manager_class EEPROM_manager;
    
    gesture_with_time_struct revolver_[8];  //actually a reversed revolver. The newest value overrides the oldest, and there are 8 values
    int barrel_;                            //counts, how many times a gesture was recorded.
                                            //barel%8 is the index of the newest value in the revolver
    
    gesture_with_time_struct history_[8];   //since the order in revolver_ isn't practical, history_ contains the last 8 gestures...
    int hist_update_;                       //... newest to oldest. hist_update_ is just for small optimization.

    bool active_;                           //true, if a gesture is active
    uint8_t gesture_[MAX_GESTURES][BONDS][9];   //9: sensors
    uint8_t n_gesture_;                         //holds, how many gestures are recorded
    
};
#endif //__GESTIC_HEADER
