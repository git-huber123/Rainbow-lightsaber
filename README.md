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
This repository consists of the following parts:
 1. The code for the ATtiny in `lightsaber_tiny.c`. Before uploading this
    code to the ATtiny, the correct clock source must be selected by
    programming the low fuse byte (run `avrdude` with `-U lfuse:w:0xe1:m`).
    This ensures that the ATtiny is clocked at 16MHz.
 2. The schematic and PCB files in the `board` directory. These files
    can be opened with gschem and gEDA PCB respectively.
 3. The Arduino library in the `libraries` directory. The library can be
    installed by copying the `lightsaber` subdirectory into the
    `libraries` directory in your sketchbook folder. See the
    [Arduino documentation](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/#manual-installation)
    for more information on this process.
 4. The Arduino code for the Nano in the `arduino_lightsaber` directory.
    This can be opened directly with the Arduino IDE.
 5. Memory programming tools in the `flash` directory. Programming the SPI
    FLASH requires an Arduino with the `memory_programmer.c` program running on
    it. To program the SPI FLASH, use `program_memory_script.py`. See the
    section [Programming the SPI FLASH](#programming-the-spi-flash)
 6. The uploader script `arduino.sh`. This program can be used to upload C code
    to an Arduino or to the ATtiny (when using another Arduino as a programmer)

## Programming the SPI FLASH
Programming the SPI FLASH will require an Arduino and a few components. Since
the FLASH chip operates at 3.3V, but the outputs of the Arduino are 5V. You
will either need a dedicated logic level shifter or a set of voltage dividers
(one each for SCK, MISO, and CS). 

Load the `memory_programmer.c` program onto the Arduino. Currently, the program
only works on an Arduino Mega, but a more versatile version will be available
soon. The Arduino will then erase the FLASH chip to prepare it for writing.

To load data onto the chip, use the program `program_memory_chip.py`. This
program takes three arguments: the serial port the Arduino is connected to, the
baud rate (115200), and the path to the file to upload.


