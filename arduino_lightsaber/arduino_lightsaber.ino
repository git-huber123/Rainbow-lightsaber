#include <lightsaber.h>
#include <Adafruit_NeoPixel.h>
#include <hueLUT.h>

#define LED_PIN 5
#define LED_COLOR 255 // Used to be strip.color(0, 0, 255) but strip.begin() hasn't been used yet and LED_COLOR isn't even used anymore

#define IMPACT_THRESHOLD 1500 // Default is 2000
#define BUZZ_FILTER_TC 100
#define BUZZ_MAX_CHANGE 150
#define BUZZ_BASELINE 60 // Controls volume, default is 50
#define PERIOD_BASELINE 833
#define NUM_LEDS 216
#define HALF_LEDS (NUM_LEDS / 2)
#define MIN_BRIGHTNESS 128

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// getBaseHue(milliseconds % (full_cycle_time_ms + 1)) and getHueOffset(led_num) are functions to check the LUT provided by hueLUT.h
// make sure generateHueLUT(milliseconds per full cycle) is called in setup() before LED_Rainbow() is ever used
// if the hueLUT needs to be changed, the code to generate a new LUT is in the GitHub repository at https://github.com/git-huber123/Rainbow-lightsaber

int percent = getBatteryPercent(); // Note that this is not exact, but approximate to the nearest 5%
unsigned long last_battery_check_ms = 0;

const unsigned long impact_cooldown = 300; // In ms
unsigned long last_impact_time = 0;

unsigned long last_sparkle_time = 0;
const unsigned long sparkle_interval = 300;
const unsigned long sparkle_length = 100;
bool need_to_sparkle = true;

unsigned long rainbow_start_time = 0;

unsigned long last_flash_ms = 0;
int current_color_for_flash = 1;  // 1 = first color, 2 = second color, for the flashing LEDS
uint32_t flashing_colors[2];


int seconds_per_full_cycle = 5; // For the rainbow cycling
int rainbow_cycle_type = 1;
int modes_in_use = 2; // Uses all the cycle types up to this, eg 2 uses types 1 and 2
// 1 = Rainbow Wave
// 2 = Whole lightsaber 1 rainbow color
// 3 = Dimmed rainbow wave with sparkles - NOT BEING USED


int LED_type = 1;  // Different LED Light actions
// UPDATE ^ ^ ^
// I won't use this because I think the color flashing is just ugly compared to the rainbow
// I will keep it though, in case I ever change my mind
// If I decide to re-add it, the collision code will have to be updated
// 1 = Rainbow
// 2 = 2-Color Flashing



Lightsaber ls;

int16_t x, y, z;

uint8_t amp_buzz = 100;
uint16_t average = 0;

// NEW FUNCTIONS
int getBatteryPercent() {
  uint16_t adc = read_battery_voltage();  // Reads Vcc using internal reference
  float voltage = (1.1 * 1023.0) / (float)adc;

  if (voltage >= 4.20) return 100;
  if (voltage >= 4.15) return 95;
  if (voltage >= 4.11) return 90;
  if (voltage >= 4.07) return 85;
  if (voltage >= 4.03) return 80;
  if (voltage >= 3.99) return 75;
  if (voltage >= 3.95) return 70;
  if (voltage >= 3.91) return 65;
  if (voltage >= 3.87) return 60;
  if (voltage >= 3.83) return 55;
  if (voltage >= 3.79) return 50;
  if (voltage >= 3.75) return 45;
  if (voltage >= 3.71) return 40;
  if (voltage >= 3.67) return 35;
  if (voltage >= 3.63) return 30;
  if (voltage >= 3.59) return 25;
  if (voltage >= 3.55) return 20;
  if (voltage >= 3.51) return 15;
  if (voltage >= 3.47) return 10;
  if (voltage >= 3.40) return 5;
  return 0;
}

bool valueInArray(int value, int* arr, int len) {
  for (int i = 0; i < len; i++) {
    if (arr[i] == value) return true;
  }
  return false;
}


void LED_Rainbow(unsigned long m, int cycle_type, bool sparkle, int percent) { // NUM_LEDS is used when you want the total amount of LEDs, HALF_LEDS is used for half the LEDs

  // Original base_hue calculation
  // uint16_t base_hue = (uint32_t)((65536UL * (m % (unsigned long)cycle_length_ms)) / cycle_length_ms);
  int cycle_length_ms = seconds_per_full_cycle * 1000;
  int base_hue = getBaseHue(m % (cycle_length_ms + 1));

  strip.setBrightness(MIN_BRIGHTNESS + ((percent / 100.0) * (255 - MIN_BRIGHTNESS)));

  if (percent <= 25) {
    strip.setBrightness(75);
    strip.fill(strip.color(255, 0, 0));
    return;
  }


  if (cycle_type == 1) {
    // strip.fill(strip.Color(255, 0, 0));
    // strip.show();
    // return;

    for (int led_num = 0; led_num < HALF_LEDS; led_num++) {

      // Each led's hue is base_hue + a step for each led
      // Original hue calculations
      // uint16_t hue_offset = (uint16_t)((led_num / HALF_LEDS) * 65536.0);
      // uint16_t hue = (base_hue - hue_offset) & 0xFFFF;

      // All of the possible base_hue values and hue_offset values are already declared, so this makes it quicker to calculate them
      uint16_t hue_offset = getHueOffset(led_num);
      uint16_t hue = (base_hue - hue_offset) & 0xFFFF;

      uint32_t color = strip.gamma32(strip.ColorHSV(hue, 240, 240));

      // Set color for bottom half (LEDs 0 to 107)
      strip.setPixelColor(led_num, color);

      // Set color for top half (LEDs 108 to 215), mirrored index
      strip.setPixelColor(NUM_LEDS - led_num - 1, color);
    }
    return;
  } else if (cycle_type == 2) {
    // strip.fill(strip.Color(0, 255, 0));
    // strip.show();
    // return;

    for (int led_num = 0; led_num < NUM_LEDS; led_num++) {

      strip.setPixelColor(led_num, strip.gamma32(strip.ColorHSV(base_hue, 240, 240)));
    }
    return;
  } 
  #if 0
  if (cycle_type == 3 && false) {
    return; // Because type 3 is not used and I don't want it even flickering into view
    // strip.fill(strip.Color(0, 0, 255));
    // strip.show();
    // return;
  
    int sparkleLEDS[7];
    for (int i = 0; i < 7; i++) {
      sparkleLEDS[i] = -5;
    }
    if (sparkle) {
      int sparkleLED = random(3, HALF_LEDS - 3); // Space for 3 on each side of the middle
      for (int i = -1; i < 1; i++) {
        sparkleLEDS[i+1] = sparkleLED + i;
      }
    }

    for (int led_num = 0; led_num < HALF_LEDS; led_num++) {
 
      int brightness_addend = 25;
      int sat = 220;
      if (valueInArray(led_num, sparkleLEDS, 3)) {
        brightness_addend = 155;
        sat = 240;
      }
      uint8_t brightness = (uint8_t)(brightness_addend); // pulse * x) + y; x = range y = offset eg pulse * 100) + 100 = 100-200
      // NOTE: Brightness = value (V in HSV)

      // Each led's hue is base_hue + a step for each led                     // bias toward red/yellow
      uint16_t hue_offset = (uint16_t)((led_num / HALF_LEDS) * 65536.0);
      uint32_t hue = (base_hue - hue_offset) & 0xFFFF;

      // Set color for bottom half (LEDs 0 to 107)
      strip.setPixelColor(led_num, strip.gamma32(strip.ColorHSV(hue, sat, brightness)));

      // Set color for top half (LEDs 108 to 215), mirrored index
      strip.setPixelColor(NUM_LEDS - led_num - 1, strip.gamma32(strip.ColorHSV(hue, sat, brightness)));
    }
  }
  #endif
  strip.show();
}

void LED_Flashes(unsigned long m, unsigned long &last_flash_time, uint32_t color1, uint32_t color2, int &current_color) {
  if (m - last_flash_time > 500) {
    if (current_color == 1) {
      strip.fill(color2);
      current_color = 2;
    } else {
      strip.fill(color1);
      current_color = 1;
    }
    strip.show();
    last_flash_time = m;
  }
}
// END OF NEW FUNCTIONS

void setup(void) {

  strip.begin();
  ls.begin();

  Serial.begin(500000);
  Serial.println("Booting lightsaber...");

  // NEW CODE
  flashing_colors[0] = strip.Color(0, 0, 255);
  flashing_colors[1] = strip.Color(255, 0, 0);
  // END OF NEW CODE



  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, 0);
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
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

  strip.setBrightness(MIN_BRIGHTNESS + ((percent / 100.0) * (255 - MIN_BRIGHTNESS)));

  for (uint8_t i = 0; i < HALF_LEDS; i++) {  // NEW FADE IN TYPE
    if (LED_type == 1) {               // If its rainbow type, the opening sequence is the rainbow fill
      delay(5);
      // Calculate hue progressing from bottom to middle
      uint16_t hue = (65536 / HALF_LEDS) * i;  // Hue from 0 to full rainbow spread over half strip

      // Set color for bottom half (LEDs 0 to 107)
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, 240, 240)));

      // Set color for top half (LEDs 108 to 215), mirrored index
      strip.setPixelColor(NUM_LEDS - 1 - i, strip.gamma32(strip.ColorHSV(hue, 240, 240)));

      if (percent <= 25) {
        strip.setBrightness(75);
        strip.fill(strip.color(255, 0, 0));
        break;
      }

      strip.show();
      ls.set_buzz_amplitude(i / 2);

    }
  }
  rainbow_start_time = millis(); // This is so that the wave effect doesn't jump, but smoothly continues after finishing the fade in
  Serial.println("Finished Setup");
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
      // Serial.println(amp_buzz);
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
    mag = (int32_t)z * (int32_t)z + (int32_t)y * (int32_t)y;
    mag = isqrt(mag);
    if ((mag - last_mag > IMPACT_THRESHOLD) || (mag - last_mag < -IMPACT_THRESHOLD)  && (m - last_impact_time > impact_cooldown)) {
      last_impact_time = m;
      rainbow_cycle_type = (rainbow_cycle_type % modes_in_use) + 1;
      Serial.println(rainbow_cycle_type);
      // Serial.println("Cycle: " + String(rainbow_cycle_type) +  ", Time: " + String(m) + ", Mag: " + String(mag - last_mag));
    }
    last_mag = mag;

    if (counter == 10) {
      int32_t mag = (int32_t)z * (int32_t)z + (int32_t)y * (int32_t)y;
      average = ((uint32_t)average * (BUZZ_FILTER_TC) + (uint32_t)isqrt(mag) * (256 - BUZZ_FILTER_TC)) >> 8;
      if (average > (BUZZ_MAX_CHANGE << 5)) average = BUZZ_MAX_CHANGE << 5;
      amp_buzz = BUZZ_BASELINE + (average >> 5);
      buzz_length = PERIOD_BASELINE - (average >> 5);
    }
  }
  
  if (m - last_sparkle_time > sparkle_interval || last_sparkle_time + sparkle_length >= m) {
    need_to_sparkle = true;
  } else {
    need_to_sparkle = false;
  }
  if (m - last_battery_check_ms >= 1000) {
    percent = getBatteryPercent(); // Note that this is not exact, but approximate to the nearest 5%
    last_battery_check_ms = m
  }

  LED_Rainbow(m, rainbow_cycle_type, need_to_sparkle, percent);
  
}
