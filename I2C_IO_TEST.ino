#include <Wire.h>

class i2cin_class
{
public:
  i2cin_class(): 
  pos_(0)
  {
  }

  template<typename T>
    i2cin_class & operator >>(T & t)
  {
    int8_t size = msg_[pos_];
    ++pos_;
    if(sizeof(t) != size)
    {
       Serial.print(size);
       Serial.print("<=>");
       Serial.print(sizeof(t));
       Serial.print(" ");
      Serial.println("type fail");
    }
    else
    {
      t = T();
      for(unsigned int i = 0; i < size; ++i)
      {
        t +=  ((T() + msg_[pos_]) << ( (size - 1 - i) * 8 ));
        ++pos_;
      }
    }
    return *this;
  }
  void read(unsigned int const n_byte)
  {
    Wire.requestFrom(2, n_byte);

    pos_ = 0;
    while(Wire.available())
    {
      msg_[pos_] = Wire.read();
      ++pos_;
    }
    
    pos_ = 0;
    
  }

  uint8_t msg_[80];
  int pos_;
} 
i2cin;


void setup()
{
  Wire.begin();
  Serial.begin(57600);
}

byte x = 0;



void loop()
{
  if (Serial.available() > 0) 
  {
    x = Serial.read() - '0';
    Wire.beginTransmission(2);
    Wire.write("x is ");
    Wire.write(x);
    Wire.endTransmission(); 
    Serial.println(x);
    if(x == 1)
    {
      uint8_t ng;

      i2cin.read(2);
      i2cin >> ng;
      Serial.print("number of gestures: ");
      Serial.println(ng);
    }
    if(x == 2)
    {
      uint8_t cg;
      long unsigned time;

      i2cin.read(7);
      i2cin >> cg;
      i2cin >> time;
      Serial.print("current gestures: ");
      Serial.println(cg);
      Serial.print("current time: ");
      Serial.println(time);
    }
    if(x == 3)
    {
      int sens[9];

      i2cin.read(27);

      for(int i = 0; i < 9; ++i)
        i2cin >> sens[i];

      for(int i = 0; i < 9; ++i)
      {
        Serial.print("sensor nr ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(sens[i]);
      }

    }
    if(x == 4 or x == 5)
    {
      unsigned long time[8];
      uint8_t g[8];
      
      i2cin.read(80);

      for(int i = 0; i < 4; ++i)
      {
        i2cin >> g[i];
        i2cin >> time[i];
      }

      for(int i = 0; i < 4; ++i)
      {
        Serial.print("last gesture nr ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(g[i]);
        Serial.print(" at time: ");
        Serial.println(time[i]);
      }

    }
    if(x == 6)
    {
      Serial.println("start recording ");
    }
    if(x == 7)
    {
      Serial.println("stop recording ");
    }
    if(x == 8)
    {
      Serial.println("reset timer");
    }
    if(x == 9)
    {
      Serial.println("delete all gestures");
    }
    if(x == 10)
    {
      Serial.println("write gestures to eeprom");
    }
        if(x == 11)
    {
      Serial.println("reset on glove");
    }
  }
}



