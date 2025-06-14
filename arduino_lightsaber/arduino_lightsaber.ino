#include <lightsaber.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 5
#define LED_COLOR strip.Color(0, 0, 255)

#define IMPACT_THRESHOLD 1500 // Default is 2000
#define BUZZ_FILTER_TC 100
#define BUZZ_MAX_CHANGE 150
#define BUZZ_BASELINE 60 // Controls volume, default is 50
#define PERIOD_BASELINE 833
#define NUM_LEDS 216
#define HALF_LEDS (NUM_LEDS / 2)
#define DEFAULT_BRIGHTNESS 128

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// NEW VAR DECLARATION
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


int rainbow_cycle_type = 1;
int modes_in_use = 2; // Uses all the cycle types up to this, eg 2 uses types 1 and 2
// 1 = Rainbow Wave
// 2 = Whole lightsaber 1 rainbow color
// 3 = Dimmed rainbow wave with sparkles


uint16_t base_hue = 0;
float seconds_per_full_cycle = 5; // For the rainbow cycling
int LED_type = 1;  // Different LED Light actions
// UPDATE ^ ^ ^
// I won't use this because I think the color flashing is just ugly compared to the rainbow
// I will keep it though, incase I ever change my mind
// If I decide to re-add it, the collision code will have to be updated
// 1 = Rainbow
// 2 = 2-Color Flashing



Lightsaber ls;

int16_t x, y, z;

uint8_t amp_buzz = 100;
uint16_t average = 0;

void setup(void) {
  strip.begin();
  strip.setBrightness(DEFAULT_BRIGHTNESS);
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
  for (uint8_t i = 0; i < HALF_LEDS; i++) {  // NEW FADE IN TYPE
    if (LED_type == 1) {               // If its rainbow type, the opening sequence is the rainbow fill
      delay(5);
      // Calculate hue progressing from bottom to middle
      uint16_t hue = (65536 / HALF_LEDS) * i;  // Hue from 0 to full rainbow spread over half strip

      // Set color for bottom half (LEDs 0 to 107)
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, 240, 240)));

      // Set color for top half (LEDs 108 to 215), mirrored index
      strip.setPixelColor(NUM_LEDS - 1 - i, strip.gamma32(strip.ColorHSV(hue, 240, 240)));

      strip.show();
      ls.set_buzz_amplitude(i / 2);
    } else if (LED_type == 2) {  // If its the color flashing, it fills in as the first color to be shown
      strip.setPixelColor(i, flashing_colors[0]);

      strip.setPixelColor(NUM_LEDS - 1 - i, flashing_colors[0]);
    delay(5);
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

// NEW FUNCTIONS ADDED BY ME
bool valueInArray(int value, int* arr, int len) {
  for (int i = 0; i < len; i++) {
    if (arr[i] == value) return true;
  }
  return false;
}


void LED_Rainbow(uint16_t base_hue, unsigned long m, float seconds_per_full_cycle, int cycle_type, bool sparkle) { // NUM_LEDS is used when you want the total amount of LEDs, HALF_LEDS is used for half the LEDs

  float cycle_length_ms = seconds_per_full_cycle * 1000.0;
  float pulse_speed = seconds_per_full_cycle / 2;

  if (cycle_type == 1) {
    // strip.fill(strip.Color(255, 0, 0));
    // strip.show();
    // return;

    for (int led_num = 0; led_num < HALF_LEDS; led_num++) {
      float position_factor = (float)led_num / HALF_LEDS;
      float pulse = 0.5 * (1.0 + sin( (2 * PI * m / (pulse_speed * 1000.0) ) + (2 * PI * position_factor) ) );
      uint8_t brightness = (uint8_t)((pulse * 100) + 155); // pulse * x) + y; x = range y = offset eg pulse * 100) + 100 = 100-200
      // NOTE: Brightness = value (V in HSV)

      // Each led's hue is base_hue + a step for each led
      float normalized = (float)led_num / HALF_LEDS;              // from 0.0 to 1.0
      float warped = pow(normalized, 0.75);                       // bias toward red/yellow
      uint16_t hue_offset = (uint16_t)(warped * 65536.0);
      uint16_t hue = (base_hue - hue_offset) & 0xFFFF;

      // Set color for bottom half (LEDs 0 to 107)
      strip.setPixelColor(led_num, strip.gamma32(strip.ColorHSV(hue, 240, brightness)));

      // Set color for top half (LEDs 108 to 215), mirrored index
      strip.setPixelColor(NUM_LEDS - led_num - 1, strip.gamma32(strip.ColorHSV(hue, 240, brightness)));
    }
    strip.show();
  } else if (cycle_type == 2) {
    // strip.fill(strip.Color(0, 255, 0));
    // strip.show();
    // return;

    for (int led_num = 0; led_num < NUM_LEDS; led_num++) {
      float position_factor = (float)led_num / NUM_LEDS;
      float pulse = 0.5 * (1.0 + sin( (2 * PI * m / (pulse_speed * 1000.0) ) + (2 * PI * position_factor) ) );
      uint8_t brightness = (uint8_t)((pulse * 100) + 155); // pulse * x) + y; x = range y = offset eg pulse * 100) + 100 = 100-200

      strip.setPixelColor(led_num, strip.gamma32(strip.ColorHSV(base_hue, 240, brightness)));
    }
    strip.show();
  } else if (cycle_type == 3) {
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
      float position_factor = (float)led_num / HALF_LEDS;
      float pulse = 0.5 * (1.0 + sin( (2 * PI * m / (pulse_speed * 1000.0) ) + (2 * PI * position_factor) ) );
      int brightness_addend = 25;
      int sat = 220;
      if (valueInArray(led_num, sparkleLEDS, 3)) {
        brightness_addend = 155;
        sat = 240;
      }
      uint8_t brightness = (uint8_t)((pulse * 100) + brightness_addend); // pulse * x) + y; x = range y = offset eg pulse * 100) + 100 = 100-200
      // NOTE: Brightness = value (V in HSV)

      // Each led's hue is base_hue + a step for each led
      float normalized = (float)led_num / HALF_LEDS;              // from 0.0 to 1.0
      float warped = pow(normalized, 0.75);                       // bias toward red/yellow
      uint16_t hue_offset = (uint16_t)(warped * 65536.0);
      uint16_t hue = (base_hue - hue_offset) & 0xFFFF;

      // Set color for bottom half (LEDs 0 to 107)
      strip.setPixelColor(led_num, strip.gamma32(strip.ColorHSV(hue, sat, brightness)));

      // Set color for top half (LEDs 108 to 215), mirrored index
      strip.setPixelColor(NUM_LEDS - led_num - 1, strip.gamma32(strip.ColorHSV(hue, sat, brightness)));
    }
  }
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
  
  float cycle_length_ms = seconds_per_full_cycle * 1000.0;
  base_hue = (uint32_t)((65536UL * (m % (unsigned long)cycle_length_ms)) / cycle_length_ms);

  if (m - last_sparkle_time > sparkle_interval || last_sparkle_time + sparkle_length >= m) {
    need_to_sparkle = true;
  } else {
    need_to_sparkle = false;
  }  

  LED_Rainbow(base_hue, m, seconds_per_full_cycle, rainbow_cycle_type, need_to_sparkle);
  
}
