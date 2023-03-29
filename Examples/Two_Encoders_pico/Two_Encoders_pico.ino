// An example sketch for two rotary encoders using Raspberry Pi Pico

/*
  Copyright (C) Martin Da Costa 2023

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
#include <EncoderMD.h>

const byte VER_MAJ = 2;   // code major version
const byte VER_MIN = 0;   // code minor version
const byte VER_PATCH = 0; // code sub-version

const byte PIN_ENC1A = 16;  // Number is GPIO reference NOT physical pin number (21)
const byte PIN_ENC1B = 17;  // Number is GPIO reference NOT physical pin number (22)
const byte PIN_ENC2A = 18;  // Number is GPIO reference NOT physical pin number (24)
const byte PIN_ENC2B = 19;  // Number is GPIO reference NOT physical pin number (25)

EncoderMD encoder1(PIN_ENC1A, PIN_ENC1B);
EncoderMD encoder2(PIN_ENC2A, PIN_ENC2B, 1);  // Third argument specifies input pullup if no external pullup implemented.

int newPos1 = 0;
int newPos2 = 0;
volatile int oldPos1 = 0;
volatile int oldPos2 = 0;
volatile byte lastPins = 0;
int minPos1 = -10;
int maxPos1 = 10;
int minPos2 = 0;
int maxPos2 = 20;

void setup()
{
  Serial.begin (115200);
  delay(2000);  // Required to ensure that serial outputs in setup are executed
  Serial << F("> Pin Change Interrupt example for encoder") << endl << endl;

  attachInterrupt(PIN_ENC1A, isr, CHANGE);
  attachInterrupt(PIN_ENC1B, isr, CHANGE);
  attachInterrupt(PIN_ENC2A, isr, CHANGE);
  attachInterrupt(PIN_ENC2B, isr, CHANGE);

  encoder1.setLimits (minPos1, maxPos1);
  encoder1.setPosition (0);
  encoder1.setWrap (0);
  encoder2.setLimits (minPos2, maxPos2);
  encoder2.setPosition (0);
  encoder2.setWrap (0);
  
  // show code version and copyright notice
  printConfig();
  
  Serial << F("> Encoder 1 position = ") << newPos1 << F("  Encoder 2 position = ") << newPos2 << endl;
}

void loop()
{
  newPos1 = encoder1.getPosition();
  newPos2 = encoder2.getPosition();
  if ((newPos1 != oldPos1) || (newPos2 != oldPos2))
  {
    Serial << F("> Encoder 1 position = ") << newPos1 << F("  Encoder 2 position = ") << newPos2 << endl;
    oldPos1 = newPos1;
    oldPos2 = newPos2;
  }
}

// The Pico only has one hardware interrupt vector per core.  Thus, a hardware interrupt could have
// originated from any of the 26 GPIO pins that have been interrupt enabled. Since, in this example, we have 4 interrupts
// from 2 seperate encoders, it is necessary to identify the encoder that has originated an interrupt.

void isr()
{
  byte pins = (digitalRead(PIN_ENC1A) | (digitalRead(PIN_ENC1B) << 1) | (digitalRead(PIN_ENC2A) << 2) | (digitalRead(PIN_ENC2B) << 3));
  // Assembles pins into a byte of data.  In this case we are only using half a byte.  Clearly one byte could manage 4 encoders.
  byte change = pins ^ lastPins;  //determine what pins have changed
  lastPins = pins;

  if (change & 0b00000011)
  {
    encoder1.encoderISR();
  }
  if (change & 0b00001100)
  {
    encoder2.encoderISR();
  }
}	

void printConfig(void) {
  // code version
  Serial << F("> code version: ") << VER_MAJ << F(".") << VER_MIN << F(".") << VER_PATCH << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;

  // copyright
  Serial << F("> © Martin Da Costa (MERG M6223) 2023") << endl
         << endl;
}