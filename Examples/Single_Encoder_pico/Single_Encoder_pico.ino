// An example sketch for a single rotary encoder using Raspberry Pi Pico

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

const byte PIN_ENCA = 16;  // Number is GPIO reference NOT physical pin number (21)
const byte PIN_ENCB = 17;  // Number is GPIO reference NOT physical pin number (22)

EncoderMD encoder(PIN_ENCA, PIN_ENCB);

int newPos = 0;
volatile int oldPos = 0;
int minPos = -10;
int maxPos = 10;

void setup() {
  Serial.begin(115200);
  delay(2000);  // Required to ensure that serial outputs in setup are executed
  Serial << F("> Pin Change Interrupt example for encoder") << endl
         << endl;

  attachInterrupt(PIN_ENCA, isr, CHANGE);
  attachInterrupt(PIN_ENCB, isr, CHANGE);

  encoder.setLimits(minPos, maxPos);
  encoder.setPosition(newPos);
  encoder.setWrap(0);

  // show code version and copyright notice
  printConfig();

  Serial << F("> Encoder position = ") << newPos << endl;
}

void loop() {
  newPos = encoder.getPosition();
  if (newPos != oldPos) {
    Serial << F("> Encoder position = ") << newPos << endl;
    oldPos = newPos;
  }
}

void isr()  // Interrupt vector
{
  encoder.encoderISR();
}

void printConfig(void) {
  // code version
  Serial << F("> code version: ") << VER_MAJ << F(".") << VER_MIN << F(".") << VER_PATCH << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;

  // copyright
  Serial << F("> © Martin Da Costa (MERG M6223) 2023") << endl
         << endl;
}