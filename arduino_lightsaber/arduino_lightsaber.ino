#include <lightsaber.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(216, LED_PIN, NEO_GRB + NEO_KHZ800);

Lightsaber ls;

int16_t x, y, z;

uint8_t amp_buzz = 100;
uint16_t average = 0;

void setup(void) {
  Serial.begin(500000);
  Serial.println("Booting lightsaber...");

  strip.begin();
  ls.begin();
  
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, 0);
  for (uint8_t i=0; i < 216; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();

  ls.config_gyro(0x4C);
  ls.config_accel(0x44);
  Serial.println("Started accelerometer");
  Serial.flush();
  ls.set_buzz_amplitude(10);
  ls.set_flash_amplitude(100);
  ls.play_from_flash(0x00000);
  for (uint8_t i=0; i < 108; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
    strip.setPixelColor(215-i, strip.Color(255, 0, 0));
    strip.show();
    ls.set_buzz_amplitude(i/2);
  }
}

// 235 -> 4750 mV
// 300 -> 3714 mV
uint8_t counter = 10;

int32_t last_mag = 0;
int32_t mag;
uint16_t buzz_length = 0;
unsigned long last_time, m;

EMPTY_INTERRUPT(PCINT2_vect);

void loop() {
  m = millis();
  if (m - last_time >= 10) {
    if (counter == 10) {
      ls.set_buzz_amplitude(amp_buzz);
      ls.set_buzz_length(buzz_length);
      last_time = m;
      counter = 0;
    }
    counter++;
    
    if (Serial.available()) {
      char c = Serial.read();
      if (c == 's') {
        Serial.println("sleeping");
        Serial.flush();
        ls.sleep();
        Serial.println("awake");
      }
    }
  
    ls.read_gyro(&x, &y, &z);
    mag = (int32_t)z*(int32_t)z + (int32_t)y*(int32_t)y;
    mag = isqrt(mag);
    if ((mag - last_mag > 2000) || (mag - last_mag < -2000)) {
      strip.fill(strip.Color(255,255,255));
      strip.show();
      strip.fill(strip.Color(255,0,0));
      strip.show();
    }
    last_mag = mag;
  
    if (counter == 10) {
      int32_t mag = (int32_t)z*(int32_t)z + (int32_t)y*(int32_t)y;
      average = ((uint32_t)average * 100 + (uint32_t)isqrt(mag)*156)>>8;
      if (average > (150<<5)) average = 150<<5;
      amp_buzz = 50 + (average>>5);
      buzz_length = 833 - (average >> 5);
    }
  }
}
