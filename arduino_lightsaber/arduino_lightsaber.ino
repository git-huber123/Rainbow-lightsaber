#include <lightsaber.h>

Lightsaber ls;

int16_t x, y, z;

uint8_t amp_buzz = 100;

void setup(void) {
  Serial.begin(500000);

  pinMode(6, OUTPUT);
  digitalWrite(6, 1);

  ls.config_gyro(0x4C);
  ls.config_accel(0x44);
  ls.set_buzz_amplitude(100);
  ls.set_flash_amplitude(100);
  ls.play_from_flash(0x30000);

  delay(3000);
  digitalWrite(6, 0);
}

void loop() {
  // Get a new normalized sensor event
  

  /* Display the results (rotation is measured in rad/s) */
  ls.read_gyro(&x, &y, &z);
  //Serial.print("Gyro X: ");
  //Serial.print(x);
  //Serial.print(" \tY: ");
  //Serial.print(y);
  //Serial.print(" \tZ: ");
  //Serial.print(z);
  //Serial.println(" radians/s ");
  //Serial.print(x);
  //Serial.print("\t");
  //Serial.print(y);
  //Serial.print("\t");
  //Serial.print(z);
  //Serial.print("\t");
  ls.read_accel(&x, &y, &z);
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.println();
  //amp_buzz++;
  //ls.set_buzz_amplitude(amp_buzz);

  delay(10);
}
