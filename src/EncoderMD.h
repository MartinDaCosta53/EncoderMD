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
    EncoderMD(byte pin1, byte pin2, bool pullup = 0);

    int getPosition();
    void setPosition(int newPosition);
    void setLimits(int minPos, int maxPos);
    void setWrap(bool wrap);
    void encoderISR(void);
	void setIncrement(int increment);

  private:
    byte _pin1, _pin2;
	bool _pullup;
    volatile int _position;
	int _increment = 1;

    int _minPos = 0;
    int _maxPos = 10;
    bool _wrap = 0;
 };
#endif
