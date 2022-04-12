# EncoderMD
 
This library provides a reliable means of translating the 2 bit Gray code sequence
that is produced by low cost rotary encoders into a position.  It does not provide
any facility for handling the push button switch often found on these encoders.

## Key Features

- Max and min limits
- Encoder wrap round
- Position setting
- Position range of -2^15 to 2^15

## Testing

The library has been tested on Arduino Uno, Nano and Mega.

## Limitations

The library requires the use of two interrupts for each instantiation.  The examples
show how Pin Change Interrupts can be used to provide this capability on all Uno and
Nano digital I/O pins.  The Mega cannot handle PCI on all pins and refernce should
be made to the data sheet when used on a Mega.  The user is advised to refer to the
relevant board data sheets to ensure correct mapping of pins to data ports.

## Functions

The following functions allow interface to the encoder library

### setPosition (int newPosition)
Allows the setting of a start position for the encoder

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
Called by the PCI routine. There is no return data.
