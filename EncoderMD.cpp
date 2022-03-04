// -----
// 
// -----

#include "EncoderMD.h"
#include "Arduino.h"


// The array holds the values -1 for the entries where a position was decremented,
// a 1 for the entries where the position was incremented
// and 0 in all the other (no change or not valid) cases.

const int8_t ENCDIRECTION[] = {
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0};           //int8_t is a signed integer type with a width of exactly 8 bits


//---------------------------------------------
// Constructor - initialises member variables

EncoderMD::EncoderMD(byte pin1, byte pin2)
{
  _pin1 = pin1;
  _pin2 = pin2;

  // Setup the input pins
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);

  byte sig1 = digitalRead(_pin1);  // At the detent value is 1 due to filter pull-up resistor
  byte sig2 = digitalRead(_pin2);  // At the detent value is 1 due to filter pull-up resistor
  _oldState = sig1 | (sig2 << 1);  // At the detent the value of the encoder should be 3

  // start with position 0;
  _position = 0;
  _positionExt = 0;
} 


int EncoderMD::getPosition()
{
  return _positionExt;
} 

void EncoderMD::setLimits(byte minPos, byte maxPos)
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

void EncoderMD::setPosition(int newPosition)
{	
    _position = ((newPosition << 2) | (_position & 0x03));
    _positionExt = newPosition;
} 

void EncoderMD::encoderISR(void)
{
  byte sig1 = digitalRead(_pin1);
  byte sig2 = digitalRead(_pin2);
  int8_t thisState = sig1 | (sig2 << 1);

  if (_oldState != thisState) {
    _position += ENCDIRECTION[(_oldState << 2) | thisState];
    _oldState = thisState;	
    _positionExt = _position >> 2;
  } 

}
  
  
  	if (_positionExt < _minPos)
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
	else if (_positionExt > _maxPos)
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
