# EncoderMD
 
This library provides a reliable means of translating the 2 bit Gray code sequence
that is produced by low cost rotary encoders into a position.  It does not provide
any facility for handling the push button switch often found on these encoders.

## Key Features

- Max and min limits
- Encoder wrap round
- Position setting
- Increment setting
- Position range of -2^15 to 2^15

## Testing

The library has been tested on Arduino UNO, NANO and MEGA with both one and two
encoders as shown in the examples.
As of March 2023, it has also been tested on a Raspberry Pi Pico using Earle Philhower's
port to the Arduino ecosystem https://github.com/earlephilhower/arduino-pico
Examples to demonstrate Pico use are also included.

## Limitations

The library requires the use of two interrupts for each encoder to be supported.
The examples show how Pin Change Interrupts can be used to provide this capability
on all UNO and NANO digital I/O pins.  The Mega cannot handle PCI on all pins and 
reference should be made to the data sheet when used on a MEGA.  The user is
advised to refer to the relevant board data sheets to ensure correct mapping of 
pins to data ports.  Information can also be found in the examples.

## Instantiation

Two variables are set during instantiation. These define the inputs pins for
the encoder.
The library assumes that, as default, the standard Bourns recommended R-C input
filter is used, which design has an intrinsic input pull up of the active low inputs.
An optional third variable can be set that will define input pull-up for when the
standard filter is not used. This variable is a boolean and is set to 1 for
input pull-up.

## Functions

The following functions allow interface to the encoder library

### setPosition (int newPosition)
Allows the setting of a start position for the encoder.
Default is zero.

### setIncrement (int increment)
Allows the amount that the position is incremented or decremented for each
change of encoder position to be set.  Default is one.

### setLimits (int minPos, int maxPos)
Sets the limits between which the encoder will operate.
Defaults minPos = 0, maxPos = 10.

### setWrap (bool wrap)
Sets the mode of operation to wrap around to the opposite limit when a limit is
reached.  Default is no wrap in which case the position will stop when a limit is
reached.

### getPosition()
Request to obtain current position. Returns an integer.

### encoderISR()
Called by the Interrupt Service routine. There is no return data.

##Using Pin Change Interrupts

| UNO Pin .| PCIE .| PCMSK .| PCINT .| ISR Vector .| uC Port .| Port Bit .| Comments         |
| :-------:| :---: | :----: | :----: | :---------: | :------: | :-------: | :--------------- |
| D0       | 2     | 2      | 16     | 2           | D        | 0         | UART Rx USB Port |
| D1       | 2     | 2      | 17     | 2           | D        | 1         | UART Tx USB Port |
| D2       | 2     | 2      | 18     | 2           | D        | 2         | INT 0            |
| D3       | 2     | 2      | 19     | 2           | D        | 3         | INT 1            |
| D4       | 2     | 2      | 20     | 2           | D        | 4         |                  |
| D5       | 2     | 2      | 21     | 2           | D        | 5         |                  |
| D6       | 2     | 2      | 22     | 2           | D        | 6         |                  |
| D7       | 2     | 2      | 23     | 2           | D        | 7         |                  |
| D8       | 0     | 0      | 0      | 0           | B        | 0         |                  |
| D9       | 0     | 0      | 1      | 0           | B        | 1         |                  |
| D10      | 0     | 0      | 2      | 0           | B        | 2         | SPI SS           |
| D11      | 0     | 0      | 3      | 0           | B        | 3         | SPI MOSI         |
| D12      | 0     | 0      | 4      | 0           | B        | 4         | SPI MISO         |
| D13      | 0     | 0      | 5      | 0           | B        | 5         | SPI SCK          |
| A0       | 1     | 1      | 8      | 1           | C        | 0         |                  |
| A1       | 1     | 1      | 9      | 1           | C        | 1         |                  |
| A2       | 1     | 1      | 10     | 1           | C        | 2         |                  |
| A3       | 1     | 1      | 11     | 1           | C        | 3         |                  |
| A4       | 1     | 1      | 12     | 1           | C        | 4         | I2C SDA          |
| A5       | 1     | 1      | 13     | 1           | C        | 5         | I2C SCL          |

Table 1 summarises the Pin Change Interrupt number associated with each of the UNO 
and NANO I/O pins. The micro-controller port and port bit that constitutes each of
these I/O bits is also shown.  It will be noticed that there are gaps in the listing
of Pin Change Interrupts that align with those Ports that do not map 8 bits to 
Arduino I/O pins.  Those port bits exist and are used for support functions such as
crystal oscillator and reset.  Needless to say, the Pin Change Interrupt on these
bits should be avoided.

Other I/O pins have other uses, such as USB interface, and so these pins may need
to be avoided depending upon application.  The pins that may need to be avoided are
identified by the Comments column.

The use of pin change interrupts is dependant upon two parts of the sketch.  These
are directly tied to the pins defined fr the encoder(s).

### void setupPCI()
This function needs to be called during setup().  It configures the various registers
for the pins that require the PCI function. Consider the single encoder example, where
pins 8 & 9 have been allocated to the encoder. Reference to Table 1 shows that these 
pins are both covered by PCI Enable 0 (PCIE0). This defines how the
PCI Change Register (PCICR) needs to be defined.  This is done in the statement

PCICR |= (1 << PCIE0);

The Pin Change Mask Register now needs to be set so as to only cause an interrupt
for a change on the pins of interest, in this case pins 8 & 9.  Table 1 shows that
pins 8 & 9 are both on PCMSK0 and that they are PCINT0 and PCINT1 respectively. The 
PCMSK0 can thus be set in these two lines:

PCMSK0 |= (1 << PCINT0);
PCMSK0 |= (1 << PCINT1);

In the example Two_Encoders, the encoders were allocated pins 4 - 7.  These are all
covered by PCIE2 and use PCMSK2 to select PCINT20 - PCINT23.

If, however, the encoders had been allocated to pins 6 - 9, Table 1 shows us that 
the setup would need to be:

PCICR |= (1 << PCIE0);    //Enable PCI on Port B
PCICR |= (1 << PCIE2);    //Enable PCI on Port D
PCMSK0 |= (1 << PCINT0);  //Set pin 8 for PCI
PCMSK0 |= (1 << PCINT1);  //Set pin 9 for PCI
PCMSK2 |= (1 << PCINT22); //Set pin 6 for PCI
PCMSK2 |= (1 << PCINT23); //Set pin 7 for PCI 

### Interrupt Service Routine
There are three Pin Change Interrupt vectors, one for each port.  Once again, the 
ISR vector relevant to each pin is shown in Table 1.  It won't have escaped your 
attention that there is consistancy between the numbers for PCIE, PCMSK & ISR Vector.
 
In the caes of the Single_Encoder example, both interrupts are on the same micro-
controller port (Port B) and an interrupt here will execute the ISR from PCI0 Interrupt
Vector. So, the ISR is called from:

ISR(PCINT0_vect){  }

In the Two_Encoder example, it is necessary to differentiate between the pins in order 
to route the interrupt to the correct instantiation of the EncoderMD library.  The code
identifies the pin that has caused the PCI and then routes it accordingly.  PIND would
become PINB for UNO pins 8 - 13 and PINC for UNO pins A0 - A5.

###MEGA Differences
Whilst the MEGA is blessed with many more I/O pins and thus more ports in the micro-
controller, there are still only three ports that have Pin Change Interrupt capability.
Some of these are not available to us due to specific hardware requirements on certain
port bits or not being used on the Arduino. In fact, there are only 19 PCI pins available
on a MEGA compared with 20 on an UNO, but there is less clashing of functions at the
user interface.  The only PCI capable pins that are lost are PCINT0 to PCINT3 if SPI 
is used. Whilst the MEGA has five external interrupts, up from the UNO / NANO's two, 
none of these clash with PCI capable pins.

| MEGA Pin .| PCIE .| PCMSK .| PCINT .| ISR Vector .| uC Port .| Port Bit .| Comments         |
| :--------:| :----:| :-----:| :-----:| :----------:| :-------:| :--------:|:---------------- |
| D0        | 1     | 1      | 8      | 1           | E        | 0         | UART Rx USB Port |
| D1        |       |        |        |             | E        | 1         | UART Tx USB Port |
| D2        |       |        |        |             | E        | 4         | INT 4            |
| D3        |       |        |        |             | E        | 5         | INT 5            |
|           |       |        |        |             |          |           |                  |
| D10       | 0     | 0      | 4      | 0           | B        | 4         |                  |
| D11       | 0     | 0      | 5      | 0           | B        | 5         |                  |
| D12       | 0     | 0      | 6      | 0           | B        | 6         |                  |
| D13       | 0     | 0      | 7      | 0           | B        | 7         |                  |
| D14       | 1     | 1      | 10     | 1           | J        | 1         |                  |
| D15       | 1     | 1      | 9      | 1           | J        | 0         |                  |
|           |       |        |        |             |          |           |                  |
| D18       |       |        |        |             | D        | 3         | INT 3            |
| D19       |       |        |        |             | D        | 2         | INT 2            |
| D20       |       |        |        |             | D        | 1         | INT 1            |
| D21       |       |        |        |             | D        | 0         | INT 0            |
|           |       |        |        |             |          |           |                  |
| D50       | 0     | 0      | 3      | 0           | B        | 3         | SPI MISO         |
| D51       | 0     | 0      | 2      | 0           | B        | 2         | SPI MOSI         |
| D52       | 0     | 0      | 1      | 0           | B        | 1         | SPI SCK          |
| D53       | 0     | 0      | 0      | 0           | B        | 0         | SPI SS           |
|           |       |        |        |             |          |           |                  |
| A8        | 2     | 2      | 16     | 2           | K        | 0         |                  |
| A9        | 2     | 2      | 17     | 2           | K        | 1         |                  |
| A10       | 2     | 2      | 18     | 2           | K        | 2         |                  |
| A11       | 2     | 2      | 19     | 2           | K        | 3         |                  |
| A12       | 2     | 2      | 20     | 2           | K        | 4         |                  |
| A13       | 2     | 2      | 21     | 2           | K        | 5         |                  |
| A14       | 2     | 2      | 22     | 2           | K        | 6         |                  |
| A15       | 2     | 2      | 23     | 2           | K        | 7         |                  |

Table 2 summarises the Pin Change Interrupts for the MEGA and also identifies the 
external interrupts.  This table can be used in the same way as table 1 is used for
the UNO.  Note that there is no commonality between the tables meaning that code 
written on the UNO is not immediately transferrable to the MEGA and vice versa.


## References

The UNO diagram can be found here

https://content.arduino.cc/assets/Pinout-UNOrev3_latest.pdf


Reference should also be made to the ATmega 328/P datasheet available from Microchip 
here

https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf

section 13.2.4 with regard to the setting of the Pin Change Interrupt Control Register
(PCICR)

For the MEGA, the pinout can be found here

https://content.arduino.cc/assets/Pinout-Mega2560rev3_latest.pdf

and the datasheet can be found here

https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf

Section 15.2.5 relates to the PCICR
