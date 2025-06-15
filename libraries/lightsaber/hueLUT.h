#ifndef HUELUT_H
#define HUELUT_H

#include <Arduino.h> // For pgm_read_word
#include <avr/pgmspace.h>

#define HALF_LEDS 108

extern const uint16_t hue_offset[HALF_LEDS] PROGMEM;

// Function to get hue offset for a given LED number
uint16_t getHueOffset(uint16_t led_num);

#endif // HUELUT_H
