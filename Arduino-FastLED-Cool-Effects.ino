#pragma GCC optimize("Os")

#include <Arduino.h>
#include <FastLED.h>

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
  #warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

/*
 * General Config
 */
#define DATA_PIN 5
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define BRIGHTNESS 225

/*
 *  Button handler
 */
#define BUTTON_MIN_DELAY 500
#define BUTTON_PIN 2

#define LED_ACTIVE_LOW   // Use this for common anode RGB LEDs or active-low LEDs
// #define LED_ACTIVE_HIGH  // Uncomment this for common cathode RGB LEDs or active-high LEDs

#define BUTTON_LED_TYPE_COMMON_ANODE
#define BUTTON_PUSH_LED_INDICATOR_PIN 8

// Uncomment / Comment the following line to enable / disable the ability to rotate between LED chain numbers
#define LED_CHAIN_TOGGLE_BUTTON

#define DEFAULT_LED_COUNT 50

#ifdef LED_CHAIN_TOGGLE_BUTTON
  #define LED_CHAIN_TOGGLE_BUTTON_PIN 3
  #define MAX_LED_COUNT 100
  #define CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN 7
  #define EEPROM_ADDRESS 0
  CRGB leds[MAX_LED_COUNT];

  #include <EEPROM.h>
#else
  #define MAX_LED_COUNT DEFAULT_LED_COUNT
  CRGB leds[DEFAULT_LED_COUNT];
#endif

#ifdef LED_ACTIVE_LOW
  #define LED_ON LOW
  #define LED_OFF HIGH
#else
  #define LED_ON HIGH
  #define LED_OFF LOW
#endif

int numLEDTotal = DEFAULT_LED_COUNT;
volatile unsigned long prevPressTime = 0;
volatile unsigned char design = 0;
volatile bool randomColoursSet = false;
volatile bool modeButtonPressed = false;
volatile unsigned long long modeButtonActivatedTill = 0;
volatile bool hasUpdatedOnButtonPress = false;

#ifdef LED_CHAIN_TOGGLE_BUTTON
  volatile bool resetButtonPressed = false;

  //declare reset function at address 0
  void (*resetFunc)(void) = 0;

  void updateEEPROM(int num) {
    EEPROM.update(EEPROM_ADDRESS, num);
  }

  void updateLEDCount() {
    numLEDTotal += DEFAULT_LED_COUNT;

    if (numLEDTotal > MAX_LED_COUNT) {
      numLEDTotal = DEFAULT_LED_COUNT;
    }

    updateEEPROM(numLEDTotal);
  }

  void resetButtonPushEvent() {
    // Minimum delay between switch presses (sharing all the buttons)
    if (millis() < prevPressTime) return;
    digitalWrite(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, LED_ON);

    prevPressTime = millis() + BUTTON_MIN_DELAY + 100000;
    resetButtonPressed = true;
  }
#endif

struct LEDData {
  uint8_t design = 0;
  uint8_t brightness = 255;
} __attribute__((packed));

LEDData ledData;

#include "functions/arrayFunctions.h"
#include "functions/randomBrightColour.h"

#include "patterns/colourSmash.h"
#include "patterns/cylon.h"
#include "patterns/demoReel100.h"
#include "patterns/fire2012.h"
#include "patterns/meteorRain.h"
#include "patterns/moveRainbowLight.h"
#include "patterns/pride.h"
#include "patterns/radiantShimmer.h"
#include "patterns/rainbow2.h"
#include "patterns/rainbowChasers.h"
#include "patterns/rainbowFirework.h"
#include "patterns/rainbowSmash.h"
#include "patterns/rainbowWaterfall.h"
#include "patterns/rainbowWaterfallHueRotate.h"
#include "patterns/rainbowWave.h"
#include "patterns/random.h"
#include "patterns/rotatingRainbowHue.h"
#include "patterns/sinusoidalBeats.h"
#include "patterns/sinusoidalBeats2.h"
#include "patterns/sinusoidalBeatsWithBlur.h"
#include "patterns/staticFullChainRainbow.h"

void buttonPushEvent() ;

unsigned long brightColours[] = {
  0xff5555, // Light Red
  0x00ff55, // Bright Green
  0x00ff22, // Vivid Green
  0x00aa00, // Dark Green
  0x5555ff, // Soft Blue
  0x3ce3b4, // Turquoise
  0xffff00, // Yellow
  0xff0000, // Red
  0xffaa00, // Orange
  0xff4400,  // Deep Orange
};
const unsigned char brightColoursLen = sizeof(brightColours) / sizeof(brightColours[0]);

unsigned long meteorColors[] = {
  0xFF4500, // Orange Red
  0xFFD700, // Gold
  0xFF8C00, // Dark Orange
  0xFFA500, // Orange
  0xFFFF00, // Yellow
  0xFF6347, // Tomato
  0xFF0000, // Red
  0xDC143C, // Crimson
  0xB22222, // Fire Brick
  0x8B0000, // Dark Red
  0x800000, // Maroon
  0xDAA520, // Golden Rod
  0xCD5C5C, // Indian Red
  0xF08080, // Light Coral
  0xE9967A, // Dark Salmon
  0xFA8072, // Salmon
  0xFFA07A, // Light Salmon
  0xFF7F50  // Coral
};
const unsigned char meteorColoursLen = sizeof(meteorColors) / sizeof(meteorColors[0]);

void shutDown(int shutDownAnimDelay) {
  for (unsigned char i = 0; i < numLEDTotal; ++i) {
    leds[i] = (CRGB)0x0;
    FastLED.show();
    FastLED.delay(shutDownAnimDelay);
  }
}

void showLEDCount(unsigned short numLEDTotal) {
  unsigned char indicators = numLEDTotal >= 10 ? numLEDTotal / 10 : numLEDTotal;

  // Fade in steps
  unsigned short stepFactor = indicators * 2;

  for (int i = 0; i < indicators; ++i) {
    leds[i] = CRGB::White;

    unsigned short step = 0;

    while(true) {
      if (step > 255) break;

      leds[i] = CRGB(step, step, step);

      FastLED.show();
      FastLED.delay(1);

      step += stepFactor;
    }
  }

  unsigned short flashDurationOnFactor = 1250;
  unsigned short flashDurationOffFactor = 625;

  for (unsigned char flash = 0; flash < indicators; ++flash) {
    // Flash the last LED
    leds[indicators - 1] = CRGB::Black;
    FastLED.show();
    FastLED.delay(flashDurationOnFactor / indicators);
    leds[indicators - 1] = CRGB::White;
    FastLED.show();

    if (flash < indicators - 1) {
      // Add a delay between flashes, but not after the last flash
      FastLED.delay(flashDurationOffFactor / indicators);
    }
  }
}

void setup() {
  delay(1000);

  srand(analogRead(A0));
  // Serial.begin(57800);

  pinMode(BUTTON_PUSH_LED_INDICATOR_PIN, OUTPUT);
  digitalWrite(BUTTON_PUSH_LED_INDICATOR_PIN, LED_OFF);

  // Button press event
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPushEvent, RISING);

  #ifdef LED_CHAIN_TOGGLE_BUTTON
    pinMode(LED_CHAIN_TOGGLE_BUTTON_PIN, INPUT);
    pinMode(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, OUTPUT);
    digitalWrite(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, LED_OFF);

    attachInterrupt(digitalPinToInterrupt(LED_CHAIN_TOGGLE_BUTTON_PIN), resetButtonPushEvent, RISING);
    numLEDTotal = EEPROM.read(EEPROM_ADDRESS);

    // If the stored value is not within our expected range, reset it to 50
    if (numLEDTotal < DEFAULT_LED_COUNT || numLEDTotal > MAX_LED_COUNT || numLEDTotal % DEFAULT_LED_COUNT != 0) {
      updateEEPROM(DEFAULT_LED_COUNT);
      numLEDTotal = DEFAULT_LED_COUNT;
    }
  #else
    numLEDTotal = DEFAULT_LED_COUNT;
  #endif

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, numLEDTotal)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);

  FastLED.clear();

  #ifdef LED_CHAIN_TOGGLE_BUTTON
    showLEDCount(numLEDTotal);
  #endif
}

void loop() {
  #ifdef LED_CHAIN_TOGGLE_BUTTON
    if (resetButtonPressed) {
      resetButtonPressed = false;

      delay(1000);
      updateLEDCount();

      digitalWrite(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, LED_ON);
      delay(50);

      resetFunc();
      return;
    }
  #endif

  if (modeButtonPressed && modeButtonActivatedTill < millis()) {
    modeButtonPressed = false;
    digitalWrite(BUTTON_PUSH_LED_INDICATOR_PIN, LED_OFF);
  }

  if (!hasUpdatedOnButtonPress) {
    hasUpdatedOnButtonPress = true;
  }

  if (ledData.design == 0) {
    rotatingRainbowHue(numLEDTotal);
    FastLED.delay(5);
    FastLED.show();
  } else if (ledData.design == 1) {
    rotatingRainbowHue(numLEDTotal);
    addGlitter(50, numLEDTotal);
    FastLED.delay(5);
    FastLED.show();
  } else if (ledData.design == 2) {
    rainbowFirework(numLEDTotal);
  } else if (ledData.design == 3) {
    staticRainbowChase(leds, numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 4) {
    dynamicRainbowChase(leds, numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 5) {
    rainbowWaterfall(30, 7, numLEDTotal);
    FastLED.show();
    FastLED.delay(30);
  } else if (ledData.design == 6) {
    rainbowWaterfallHueRotate(30, 7, numLEDTotal);
    FastLED.show();
    FastLED.delay(30);
  } else if (ledData.design == 7) {
    reverseRainbowWaterfall(30, 7, numLEDTotal);
    FastLED.show();
    FastLED.delay(30);
  } else if (ledData.design == 8) {
    reverseRainbowWaterfallHueRotate(30, 7, numLEDTotal);
    FastLED.show();
    FastLED.delay(30);
  } else if (ledData.design == 9) {
    moveRainbowLight(numLEDTotal, leds);
  } else if (ledData.design == 10) {
    staticFullChainRainbow(leds, numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 11) {
    staticFullChainRainbow(leds, numLEDTotal);
    addGlitter(50, numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 12) {
    rainbowWave(10, numLEDTotal);
  } else if (ledData.design == 13) {
    fill_rainbow(leds, numLEDTotal, 0, 7);
    FastLED.show();
  } else if (ledData.design == 14) {
    fill_rainbow(leds, numLEDTotal, 0, 7);
    addGlitter(50, numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 15) {
    rainbow2(numLEDTotal, 6);
    FastLED.show();
  } else if (ledData.design == 16) {
    rainbowSmash(numLEDTotal);
    FastLED.show();
    FastLED.delay(15);
  } else if (ledData.design == 17) {
    colourSmash(numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 18) {
    colourSmash2(numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 19) {
    colourSmash(numLEDTotal);
    addGlitter(50, numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 20) {
    colourSmash2(numLEDTotal);
    addGlitter(50, numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 21) {
    sinusoidalBeats(numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 22) {
    sinusoidalBeats(numLEDTotal);
    addGlitter(50, numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 23) {
    sinusoidalBeatsWithBlur(numLEDTotal);
  } else if (ledData.design == 24) {
    sinusoidalBeats2(numLEDTotal);
  } else if (ledData.design == 25) {
    radiantShimmer(numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 26) {
    rotateBrightColoursSmooth(brightColours, brightColoursLen, numLEDTotal);
  } else if (ledData.design == 27 || ledData.design == 28 || ledData.design == 29) {
    if (!randomColoursSet) {
      randomColoursSet = true;
      arrayShuffleUniqColoursEffect(brightColours, brightColoursLen, leds, numLEDTotal);
    }
    FastLED.show();
  } else if (ledData.design == 30) {
      int meteorColourIndex = rand() % meteorColoursLen;
      uint32_t meteorColour = meteorColors[meteorColourIndex];
      meteorRain(meteorColour, numLEDTotal);
  } else if (ledData.design == 31) {
    pride(numLEDTotal);
  } else if (ledData.design == 32) {
    cylon(numLEDTotal);
  } else if (ledData.design == 33) {
    demoReel100(numLEDTotal);
  } else if (ledData.design == 34) {
    fire2012(numLEDTotal);
  } else if (ledData.design == 35) {
    fill_solid(leds, numLEDTotal, 0xff5010);
    FastLED.show();
  } else if (ledData.design == 36) {
    fill_solid(leds, numLEDTotal, 0x008080);
    FastLED.show();
  } else if (ledData.design == 37) {
    fill_solid(leds, numLEDTotal, 0x32cd32);
    FastLED.show();
  } else if (ledData.design == 38) {
    fill_solid(leds, numLEDTotal, 0x00ffff);
    FastLED.show();
  } else if (ledData.design == 39) {
    fill_solid(leds, numLEDTotal, 0xff50d6);
    FastLED.show();
  } else if (ledData.design == 40) {
    fill_solid(leds, numLEDTotal, 0xff00ff);
    FastLED.show();
  } else if (ledData.design == 41) {
    fill_solid(leds, numLEDTotal, 0xffff00);
    FastLED.show();
  } else if (ledData.design == 42) {
    fill_solid(leds, numLEDTotal, 0xff2200);
    FastLED.show();
  } else if (ledData.design == 43) {
    fill_solid(leds, numLEDTotal, 0x00ff00);
    FastLED.show();
  } else if (ledData.design == 44) {
    fill_solid(leds, numLEDTotal, 0x3ce3b4);
    FastLED.show();
  } else if (ledData.design == 45) {
    fill_solid(leds, numLEDTotal, 0xff0011);
    FastLED.show();
  } else if (ledData.design == 46) {
    rotateBrightColours(brightColours, brightColoursLen, numLEDTotal);
  } else {
    shutDown(10);
  }
}

void buttonPushEvent() {
  // Prevent skipping effects
  if (!hasUpdatedOnButtonPress) return;

  // Minimum delay between switch presses (sharing all the buttons)
  if (millis() < prevPressTime) return;

  prevPressTime = millis() + BUTTON_MIN_DELAY;
  design = (design + 1) % 48;
  ledData.design = design;

  digitalWrite(BUTTON_PUSH_LED_INDICATOR_PIN, LED_ON);
  modeButtonPressed = true;
  modeButtonActivatedTill = millis() + 500;
  hasUpdatedOnButtonPress = false;

  randomColoursSet = false;
  colourSmashData.colourSmashInitialized = false;

  FastLED.setBrightness(BRIGHTNESS);
}
