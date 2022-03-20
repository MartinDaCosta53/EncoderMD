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

/*
      3rd party libraries needed for compilation: (not for binary-only distributions)

      Streaming   -- C++ stream style output, v5, (http://arduiniana.org/libraries/streaming/)
 */
 
#include <Streaming.h>
#include "EncoderMD.h"

const byte PIN_ENC1A = 4;
const byte PIN_ENC1B = 5;
const byte PIN_ENC2A = 6;
const byte PIN_ENC2B = 7;

EncoderMD encoder1(PIN_ENC1A, PIN_ENC1B);
EncoderMD encoder2(PIN_ENC2A, PIN_ENC2B);

byte newPos1;
byte newPos2;
volatile byte oldPos1 = 0;
volatile byte oldPos2 = 0;
volatile byte lastPins = 0;

void setupPCI()
{
  cli();  //Disable interrupts
  PCICR |= (1 << PCIE2);  //Enable PCI on Port D
  PCMSK2 |= (1 << PCINT20);  //Set pin 4 for PCI
  PCMSK2 |= (1 << PCINT21);  //Set pin 5 for PCI
  PCMSK2 |= (1 << PCINT22);  //Set pin 6 for PCI
  PCMSK2 |= (1 << PCINT23);  //Set pin 7 for PCI
  sei();  //Enable interrupts
}

void setup()
{
  Serial.begin (115200);
  Serial << F("> Pin Change Interrupt example for encoder") << endl << endl;

  setupPCI();

  encoder1.setLimits (0, 10);
  encoder1.setPosition (0);
  encoder2.setLimits (0, 10);
  encoder2.setPosition (0);
}

void loop()
{
  newPos1 = encoder1.getPosition();
  newPos2 = encoder2.getPosition();
  if ((newPos1 != oldPos1) || (newPos2 != oldPos2))
  {
    Serial << F("> Encoder 1 position = ") << newPos1 << F(" Encoder 2 position = ") << newPos2 << endl;
    oldPos1 = newPos1;
    oldPos2 = newPos2;
  }
}

ISR(PCINT2_vect)
{
  byte pins = PIND & 0b11110000;  //read port D inpur register and mask for pins of interest 
  byte change = pins ^ lastPins;  //determine what pins have changed
  lastPins = pins;

  if (change & 0b00110000)
  {
    encoder1.encoderISR();
  }
  if (change & 0b11000000)
  {
    encoder2.encoderISR();
  }
}
