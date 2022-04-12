// -----
// EncoderMD.h - Library for using rotary encoders.
// This class is implemented for use with the Arduino environment.
//
//

#ifndef EncoderMD_h
#define EncoderMD_h

#include "Arduino.h"

class EncoderMD
{
  public:
    // ----- Constructor -----
    EncoderMD(byte pin1, byte pin2);

    int getPosition();
    void setPosition(int newPosition);
    void setLimits(int minPos, int maxPos);
    void setWrap(bool wrap);
    void encoderISR(void);

  private:
    byte _pin1, _pin2;
    volatile int _position;

    int _minPos = 0;
    int _maxPos = 10;
    bool _wrap = 0;
    unsigned int _sequence;

};
#endif
