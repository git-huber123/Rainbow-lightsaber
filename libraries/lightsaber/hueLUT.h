#ifndef HUELUT_H
#define HUELUT_H

#include <Arduino.h>
#include <avr/pgmspace.h>

#define CYCLE_LENGTH_MS 5000
#define HALF_LEDS 108

extern const uint16_t base_hue[CYCLE_LENGTH_MS + 1] PROGMEM;
extern const uint16_t hue_offset[HALF_LEDS] PROGMEM;

uint16_t getBaseHue(uint16_t m);
uint16_t getHueOffset(uint16_t led_num);

#endif
