// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.11.2012 18:38:51 CET
// File:    eeprom_manager.hpp

#ifndef __EEPROM_MANAGER_HEADER
#define __EEPROM_MANAGER_HEADER

#include "Arduino.h"
#include "EEPROM.h"

class EEPROM_manager_class
{
public:
    EEPROM_manager_class(): n_adr(9)
    {}
    int adress(int const i, int const j, int const k) const
    {
        return 10 + i * BONDS * 9 + j * 9 + k;
    }
    
    void read(uint8_t & n_gesture, uint8_t gesture[MAX_GESTURES][BONDS][9]) //MAX_GESTURE defined in gestic.hpp
    {
        n_gesture = EEPROM.read(n_adr);
        for(int i = 0; i < n_gesture; ++i)
        {
            for(uint8_t j = 0; j < BONDS; ++j)
            {
                for(uint8_t k = 0; k < 9; ++k)
                {
                    gesture[i][j][k] = EEPROM.read(adress(i, j, k));
                }
            }
        }
    }
    void write(uint8_t const n_gesture, uint8_t const gesture[MAX_GESTURES][2][9]) const
    {
        EEPROM.write(n_adr, n_gesture);
        for(int i = 0; i < n_gesture; ++i)
        {
            for(uint8_t j = 0; j < BONDS; ++j)
            {
                for(uint8_t k = 0; k < 9; ++k)
                {
                    EEPROM.write(adress(i, j, k), byte(gesture[i][j][k]));
                }
            }
        }
    }
    void remove_all()
    {
        EEPROM.write(n_adr, 0);
    }
private:
    uint8_t n_adr;  //adress, where the number of gestures is saved
};


#endif //__EEPROM_MANAGER_HEADER
