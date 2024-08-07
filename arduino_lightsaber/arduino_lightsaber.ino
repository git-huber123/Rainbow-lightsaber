#include <lightsaber.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, LED_PIN, NEO_GRB + NEO_KHZ800);

Lightsaber ls;

int16_t x, y, z;

uint8_t amp_buzz = 100;
uint16_t average = 0;

void setup(void) {
  Serial.begin(500000);

  strip.begin();
  strip.show();
  for (uint8_t i=0; i < 144; i++) strip.setPixelColor(i, strip.Color(255, 0, 0));
  strip.show();
  
  pinMode(6, OUTPUT);
  digitalWrite(6, 1);

  ls.config_gyro(0x4C);
  ls.config_accel(0x44);
  ls.set_buzz_amplitude(100);
  ls.set_flash_amplitude(100);
  //ls.play_from_flash(0x00000);

  delay(1000);
  digitalWrite(6, 0);

}


void loop() {
  // Get a new normalized sensor event
  

  /* Display the results (rotation is measured in rad/s) */
  ls.read_gyro(&x, &y, &z);
  Serial.print("Gyro X: ");
  Serial.print(x);
  Serial.print(" \tY: ");
  Serial.print(y);
  Serial.print(" \tZ: ");
  Serial.print(z);
  int32_t mag = (int32_t)x*(int32_t)x + (int32_t)y*(int32_t)y;
  average = ((uint32_t)average * 100 + (uint32_t)isqrt(mag)*156)>>8;
  if (average > (150<<5)) average = 150<<5;
  amp_buzz = 20 + (average>>5);
  Serial.print("Amplitude ");
  Serial.println(amp_buzz);
  ls.set_buzz_amplitude(amp_buzz);

  //Serial.println(" radians/s ");
  //Serial.print(x);
  //Serial.print("\t");
  //Serial.print(y);
  //Serial.print("\t");
  //Serial.print(z);
  //Serial.print("\t");
  //ls.read_accel(&x, &y, &z);
  //Serial.print(x);
  //Serial.print("\t");
  //Serial.print(y);
  //Serial.print("\t");
  //Serial.print(z);
  //Serial.println();
  //amp_buzz++;

  delay(100);
}
