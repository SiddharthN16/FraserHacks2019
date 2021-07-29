#include "FastLED.h"                                          // FastLED library.
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <movingAvg.h>

MPU6050 mpu6050(Wire);
long timer = 0;

movingAvg avgGYRO(5);

int inPin = 3;         // the number of the input pin

int state = LOW;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers





uint8_t thishue = 0;                                          // Starting hue value.
uint8_t deltahue = 10;
#define LED_DT 5                                             // Data pin to connect to the strip.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and BGR for APA102
#define LED_TYPE WS2812B                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 25                                           // Number of LED's.
#define MAX_BRIGHTNESS 128      // Thats full on, watch the power!
#define MIN_BRIGHTNESS 1
int BRIGHTNESS = 100;
int tempBright = 100;
// set to a minimum of 25%
struct CRGB leds[NUM_LEDS];
// Initialize our LED array.

bool toggle = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  avgGYRO.begin();
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);        // Use this for WS2812B
  FastLED.setBrightness(MIN_BRIGHTNESS);


  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {

  reading = digitalRead(inPin);

  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH) {
      state = LOW;
    }
    else {
      state = HIGH;

    }
  }

  if (state) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].setRGB(255, 165, 20);
    }
    FastLED.show();
  }
  else {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
  }
  previous = reading;

  int BRIGHTNESS = constrain(tempBright, 0, 120);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(255, 165, 20);
  }


  mpu6050.update();

  if (millis() - timer > 200) {

    timer = millis();
  }







  EVERY_N_MILLISECONDS(5) {                           // FastLED based non-blocking routine to update/display the sequence.
    FastLED.setBrightness(BRIGHTNESS);
  }
  if (mpu6050.getGyroZ() > 50) {
    tempBright += 1;

  }
  if (mpu6050.getGyroZ() < -50) {
    tempBright -= 1;


  }

}
