// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.11.2012 13:04:34 CET
// File:    i2c_enum.h

#ifndef __I2C_ENUM_HEADER
#define __I2C_ENUM_HEADER

#define GLOVE_ADRESS 2

enum i2c_glove_enum
{
      get_num_of_gestures = 1
    , get_current_gesture
    , get_raw_values
    , get_last_gestures_newer
    , get_last_gestures_older
    , record_gesture
    , stop_record
    , reset_time
    , delete_all_gestures
    , write_gestures_to_eeprom
    , reset_glove
};

#endif //__I2C_ENUM_HEADER
