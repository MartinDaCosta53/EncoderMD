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
  void setLimits(byte minPos, byte maxPos);
  void setWrap(bool wrap);
  void encoderISR(void);

private:
  byte _pin1, _pin2; 
  volatile int8_t _oldState;
  volatile int _position;        // Internal position (4 times _positionExt)
  volatile int _positionExt;     // External position 
  byte _minPos = 0;
  byte _maxPos = 10;
  bool _wrap = 0;

};
#endif
