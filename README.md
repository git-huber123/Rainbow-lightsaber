Lightsaber prop
===============

This repository contains the code, Arduino libraries, schematics, and 
PCB files for a lightsaber prop. The goal of this project is to make a
kit for a maker club I am part of. The plan for the project includes
LED light strips for the "blade", an accelerometer, and sound effects 
that respond to motion. 

## Technical overview
The main code of the lightsaber runs on an Arduino Nano, which 
interfaces with the accelerometer and can directly control the LED 
lights in the blade. However, the Nano is not directly responsible for
generating the audio. Instead, the Nano sends commands to an ATtiny85,
which generates the audio accordingly. The ATtiny can read sound effects
from a flash chip and write the audio data to a DAC. 

The ATtiny also has the characteristic buzzing sound of a lightsaber 
stored in its program memory. When the accelerometer senses rotation,
the Nano can command the ATtiny to change the amplitude of the buzzing
accordingly.

More details about this project can be found on
[Hackaday](https://hackaday.io/project/195924-lightsaber-prop-with-sound-effects)
and in my [blog post](https://mnigmann.blogspot.com/2024/05/lightsaber-prop-first-prototype.html).

## Parts of this repository
This repository consists of four main parts:
 1. The code for the ATtiny in `lightsaber_tiny.c`
 2. The schematic and PCB files in the `board` directory. These files
    can be opened with gschem and gEDA PCB respectively.
 3. The Arduino library in the `libraries` directory. The library can be
    installed by copying the `lightsaber` subdirectory into the
    `libraries` directory in your sketchbook folder. See the
    [Arduino documentation](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/#manual-installation)
 4. The Arduino code for the Nano in the `arduino_lightsaber` directory.
    This can be opened directly with the Arduino IDE.

The file `lightsaber_nano.c` is a test file that can also be uploaded to
the Arduino Nano. This program varies the amplitude of the buzzing sound
as the accelerometer is rotated and can be used to test the audio samples
stored in the flash chip.
