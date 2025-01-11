#ifndef _LIGHTSABER_H
#define _LIGHTSABER_H

#include "Arduino.h"

#define AUDIO_COMMAND_AMP_BUZZ      0x0000
#define AUDIO_COMMAND_AMP_FLASH     0x1000
#define AUDIO_COMMAND_LENGTH_BUZZ   0x3000
#define AUDIO_COMMAND_FLASH_LOW     0x4000
#define AUDIO_COMMAND_FLASH_HIGH    0x5000
#define AUDIO_COMMAND_POWER_DOWN    0x7000

class Lightsaber {
public:
    // Constructor
    Lightsaber(void);
    void begin();
    // Configuration for LSM6DS
    void config_accel(uint8_t config);
    void config_gyro(uint8_t config);
    // Reading data from LSM6DS
    void read_accel(int16_t *x, int16_t *y, int16_t *z);
    void read_gyro(int16_t *x, int16_t *y, int16_t *z);
    // Writing to the ATtiny
    void communicate(uint16_t data);
    void set_buzz_amplitude(uint8_t amp);
    void set_flash_amplitude(uint8_t amp);
    void set_buzz_length(uint16_t length);
    void play_from_flash(uint32_t addr);
    // Power management
    uint16_t read_battery_voltage();
    void sleep();
private:
    // I2C functions
    void start_at_addr(uint8_t addr);
    void write_bytes(uint8_t n_bytes);
    void read_bytes(uint8_t n_bytes);
    // Buffer for I2C data
    uint8_t i2c_buf[8];
};

uint16_t isqrt(uint32_t x);

#endif
