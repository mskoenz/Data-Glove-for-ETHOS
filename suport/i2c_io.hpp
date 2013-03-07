// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.11.2012 15:47:58 CET
// File:    i2c_io.hpp

#ifndef __I2C_IO_HEADER
#define __I2C_IO_HEADER

class i2cout_class
{
    public:
    i2cout_class(): pos_(0)
    {
    }
    
    template<typename T>
    i2cout_class & operator <<(T const t)
    {
        msg_[pos_] = sizeof(t);
        ++pos_;
        for(uint8_t i = 0; i < sizeof(t); ++i)
        {
            msg_[pos_] = t >> ( (sizeof(t) - 1 - i) * 8 );
            ++pos_;
        }
        return *this;
    }
    void str()
    {
        Wire.write(msg_, pos_);
        pos_ = 0;
    }
    
    uint8_t msg_[100];
    int pos_;
} i2cout;

class i2cin_class
{
    public:
    i2cin_class(): pos_(0)
    {
    }
    
    template<typename T>
    i2cin_class & operator >>(T & t)
    {
        uint8_t size = msg_[pos_];
        ++pos_;
        
        if(sizeof(t) != size)
        {
            Serial.print(size);
            Serial.print("<->");
            Serial.print(sizeof(t));
            Serial.print(" ");
            Serial.println("type fail");
        }
        else
        {
            t = T();
            for(uint8_t i = 0; i < size; ++i)
            {
                t += (T(msg_[pos_]) << ( (sizeof(t) - 1 - i) * 8 ));
                ++pos_;
            }
        }
        return *this;
    }
    void read(uint8_t const n_byte)
    {
        Wire.requestFrom(uint8_t(2), n_byte);
        pos_ = 0;
        while(Wire.available())
        {
            msg_[pos_] = Wire.read();
            ++pos_;
        }
        pos_ = 0;
    }
    
    uint8_t msg_[100];
    int pos_;
} i2cin;


#endif //__I2C_IO_HEADER
