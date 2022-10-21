// -----
//
// -----

#include "EncoderMD.h"
#include "Arduino.h"


// The array holds the values -1 for the entries where a position was decremented,
// a 1 for the entries where the position was incremented
// and 0 in all the other (no change or not valid) cases.

const bool ENCVALIDITY[] = {
  0, 1, 1, 0,
  1, 0, 0, 1,
  1, 0, 0, 1,
  0, 1, 1, 0
};


//---------------------------------------------
// Constructor - initialises member variables

EncoderMD::EncoderMD(byte pin1, byte pin2, bool pullup)
{
  _pin1 = pin1;
  _pin2 = pin2;
  _pullup = pullup;

  // Setup the input pins
  if (_pullup == 0)
  {
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  }
  else
  {
	pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
  }	

  byte sig1 = digitalRead(_pin1);  // At the detent value is 1 due to filter pull-up resistor
  byte sig2 = digitalRead(_pin2);  // At the detent value is 1 due to filter pull-up resistor
  //  _oldState = sig1 | (sig2 << 1);  // At the detent the value of the encoder should be 3

  // start with position 0;
  _position = 0;

}


int EncoderMD::getPosition()
{
  return _position;
}

void EncoderMD::setLimits(int minPos, int maxPos)
{
  if (maxPos > minPos)
  {
    _minPos = minPos;
    _maxPos = maxPos;
  }
  encoderISR();
}

void EncoderMD:: setWrap(bool wrap)
{
  _wrap = wrap;
}

void EncoderMD:: setIncrement(int increment)
{
	_increment = increment;
}

void EncoderMD::setPosition(int newPosition)
{
  _position = newPosition;
}

void EncoderMD::encoderISR(void)
{
  static byte _sequence = 0x03;
  byte sig1 = digitalRead(_pin1);
  byte sig2 = digitalRead(_pin2);
  byte _state = ((_sequence & 0x03) << 2) | (sig1 | (sig2 << 1));

  if (ENCVALIDITY[_state])  {
    _sequence = (_sequence << 4) | _state;
    if ((_sequence ) == 0x2b) (_position = _position + _increment);
    if ((_sequence ) == 0x17) (_position = _position - _increment);
  }

  if (_position < _minPos)
  {
    if (_wrap)
    {
      setPosition(_maxPos);
    }
    else
    {
      setPosition(_minPos);
    }
  }
  else if (_position > _maxPos)
  {
    if (_wrap)
    {
      setPosition(_minPos);
    }
    else
    {
      setPosition(_maxPos);
    }
  }
}

// End
