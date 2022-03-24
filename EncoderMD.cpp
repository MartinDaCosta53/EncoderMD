/*

  Copyright (C) Martin Da Costa 2022

  This work is licensed under the:
      Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit:
      http://creativecommons.org/licenses/by-nc-sa/4.0/
   or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

   License summary:
    You are free to:
      Share, copy and redistribute the material in any medium or format
      Adapt, remix, transform, and build upon the material

    The licensor cannot revoke these freedoms as long as you follow the license terms.

    Attribution : You must give appropriate credit, provide a link to the license,
                  and indicate if changes were made. You may do so in any reasonable manner,
                  but not in any way that suggests the licensor endorses you or your use.

    NonCommercial : You may not use the material for commercial purposes. **(see note below)

    ShareAlike : If you remix, transform, or build upon the material, you must distribute
                 your contributions under the same license as the original.

    No additional restrictions : You may not apply legal terms or technological measures that
                                 legally restrict others from doing anything the license permits.

   ** For commercial use, please contact the original copyright holder(s) to agree licensing terms

    This software is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

*/
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
  0, 1, -1, 0
};           //int8_t is a signed integer type with a width of exactly 8 bits


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
  int8_t change = ENCDIRECTION[(_oldState << 2) | thisState];

  if (change != 0)  {
    _position += change;
    _oldState = thisState;
    _positionExt = _position >> 2;
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
