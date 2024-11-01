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
  //strip.show();
  
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, 0);
  for (uint8_t i=0; i < 216; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  //digitalWrite(6, 1);

  ls.config_gyro(0x4C);
  ls.config_accel(0x44);
  Serial.println("Started accelerometer");
  Serial.flush();
  ls.set_buzz_amplitude(10);
  ls.set_flash_amplitude(100);
  ls.play_from_flash(0x00000);
  for (uint8_t i=0; i < 108; i++) {
    //uint16_t voltage = ls.read_battery_voltage();
    //Serial.println(voltage);
    strip.setPixelColor(i, strip.Color(255, 0, 0));
    strip.setPixelColor(215-i, strip.Color(255, 0, 0));
    strip.show();
  }
  //delay(2000);
  //ls.set_buzz_amplitude(20);

  //delay(5000);
  //Serial.println("sleeping");
  //Serial.flush();
  //ls.sleep();

  //digitalWrite(6, 0);

}

// 235 -> 4750 mV
// 300 -> 3714 mV
uint8_t counter = 10;

EMPTY_INTERRUPT(PCINT2_vect);

void loop() {
  // Get a new normalized sensor event

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 's') {
      Serial.println("sleeping");
      Serial.flush();
      ls.sleep();
      //UDR0 = ((SREG & 0x80) ? 'i' : 'd');
      Serial.println("awake");
    }
  }

  /* Display the results (rotation is measured in rad/s) */
  Serial.print("r: ");
  ls.read_accel(&x, &y, &z);
  int32_t mag = (int32_t)z*(int32_t)z + (int32_t)y*(int32_t)y + (int32_t)x*(int32_t)x;
  mag = isqrt(mag);
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.print("\t");
  Serial.println(mag);
  //Serial.print("\t");
  if (counter == 10) {
    ls.read_gyro(&x, &y, &z);
    //Serial.print("Gyro X: ");
    //Serial.print(x);
    //Serial.print(" \tY: ");
    //Serial.print(y);
    //Serial.print(" \tZ: ");
    //Serial.println(z);
    int32_t mag = (int32_t)z*(int32_t)z + (int32_t)y*(int32_t)y;
    average = ((uint32_t)average * 100 + (uint32_t)isqrt(mag)*156)>>8;
    if (average > (150<<5)) average = 150<<5;
    amp_buzz = 10 + (average>>5);
    //Serial.print("Amplitude ");
    //Serial.println(amp_buzz);
    //Serial.print(amp_buzz);
    //Serial.print("\t");
    ls.set_buzz_amplitude(amp_buzz);
  
    uint16_t voltage = ls.read_battery_voltage();
    //Serial.print("Voltage: ");
    //Serial.println(voltage);
    //Serial.println(voltage);
    counter = 0;
  }
  counter++;

  delay(10);
}
