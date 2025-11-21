#pragma GCC optimize("Os")

#define TOTAL_PATTERNS 51

#include <Arduino.h>
#include <EEPROM.h>
#include "boardConfig.h"

#if !defined(BOARD_ESP8266) && !defined(BOARD_ARDUINO) && !defined(BOARD_STM32F103C8)
  #warning "Neither BOARD_ESP8266 nor BOARD_ARDUINO is defined. Using Arduino by default"
  #define BOARD_ARDUINO
#endif

#ifndef min
  #define min(a,b) ((a)<(b)?(a):(b))
#endif

#include "FastLED.h"

using namespace fl;

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
  #warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Change configurations in userConfig.h
#include "userConfig.h"

// Internal Configuration Section: This part of the code defines critical settings for LED control
// logic based on the system's configuration.
// It is automatically adjusted according to the hardware setup specified in userConfig.h.
// Users should not modify this section to ensure the proper functioning of LED behaviors as intended by the design.
// Alterations here may disrupt the operation and compatibility with different LED types.

// Restart MCU a little before millis() overflows (~49.7 days)
#define MILLIS_OVERFLOW_RESTART_MARGIN_MS 60000UL  // in ms; 1 minute before overflow
#define MILLIS_OVERFLOW_RESTART_THRESHOLD ((uint64_t)(1ULL << 32) - MILLIS_OVERFLOW_RESTART_MARGIN_MS)

#if DEFAULT_LED_COUNT < 1
  #error "DEFAULT_LED_COUNT cannot be 0."
#endif

#if DEFAULT_LED_COUNT > 255
  #error "DEFAULT_LED_COUNT cannot exceed 255. Ideally it should be <= 200"
#endif

#ifdef LED_CHAIN_TOGGLE_BUTTON
  #if DEFAULT_LED_COUNT > MAX_LED_COUNT
    #error "DEFAULT_LED_COUNT cannot exceed MAX_LED_COUNT"
  #endif

  #if MAX_LED_COUNT > 255
    #error "MAX_LED_COUNT cannot exceed 255. Ideally it should be <= 200 and a multiple of DEFAULT_LED_COUNT (50 -> 100, 150, 200)"
  #endif
#endif

#if defined(BOARD_ESP8266)
  #define EEPROM_SAVE_VALUE(address, value) EEPROM.write(address, value); EEPROM.commit();
#else
  #define EEPROM_SAVE_VALUE(address, value) EEPROM.update(address, value);
#endif

#ifdef LED_ACTIVE_LOW
  #define LED_ON LOW
  #define LED_OFF HIGH
#else
  #define LED_ON HIGH
  #define LED_OFF LOW
#endif

volatile struct LEDData {
  uint32_t modeButtonActivatedTill = 0;
  uint32_t prevPressTime = 0;
  uint32_t scheduledEEPROMWriteTime = 0;
  uint8_t numLEDTotal = DEFAULT_LED_COUNT;
  uint8_t design = 0;
  bool hasUpdatedOnButtonPress = false;
  bool randomColoursSet = false;
  bool modeButtonPressed = false;
  bool ledChainToggleButtonPressed = false;
  bool designChangeDetected = false;
  bool eepromWritePendingDelay = false;
  bool flagWriteLEDCountToEEPROM = false;
} ledData;

// Declare reset function at address 0
#ifndef BOARD_ESP8266
  void (*resetFunc)(void) = 0;
#endif

void restartMCU() {
  #if defined(BOARD_ESP8266)
    ESP.restart();
  #else
    resetFunc();
  #endif
}

#ifdef LED_CHAIN_TOGGLE_BUTTON
  CRGB leds[MAX_LED_COUNT];

  void writeLEDCountToEEPROM(uint16_t num) {
    EEPROM_SAVE_VALUE(EEPROM_LED_COUNT_ADDRESS_1, num);
    EEPROM_SAVE_VALUE(EEPROM_LED_COUNT_ADDRESS_2, num);
    EEPROM_SAVE_VALUE(EEPROM_LED_COUNT_ADDRESS_3, num);
    EEPROM_SAVE_VALUE(EEPROM_LED_COUNT_ADDRESS_4, num);
  }

  /**
  * Retrieves the LED count from four specific EEPROM addresses, applying redundancy to ensure
  * data integrity. The function identifies the most common value among these addresses as the correct
  * LED count, providing resilience against data corruption. In the absence of a clear majority
  * (indicating potential corruption), the EEPROM locations are reset to a default LED count,
  * and this default count is returned.
  *
  * @return The validated LED count, or a default count if the stored values are inconsistent or invalid.
  */
  uint8_t readLEDCountFromEEPROM() {
    const uint8_t addresses[4] = {
      EEPROM_LED_COUNT_ADDRESS_1,
      EEPROM_LED_COUNT_ADDRESS_2,
      EEPROM_LED_COUNT_ADDRESS_3,
      EEPROM_LED_COUNT_ADDRESS_4
    };

    return readCommonValueFromEEPROM(addresses, 4, MAX_LED_COUNT, DEFAULT_LED_COUNT);
  }

  void updateLEDCount() {
    const uint8_t prevValue = ledData.numLEDTotal;
    ledData.numLEDTotal += DEFAULT_LED_COUNT;

    if (ledData.numLEDTotal < prevValue || ledData.numLEDTotal > MAX_LED_COUNT) {
      ledData.numLEDTotal = DEFAULT_LED_COUNT;
    }

    writeLEDCountToEEPROM(ledData.numLEDTotal);
  }

  #ifdef BOARD_ESP8266
    void IRAM_ATTR resetButtonPushEvent() {
  #else
    void resetButtonPushEvent() {
  #endif
      // Minimum delay between switch presses (sharing all the buttons)
      if (millis() < ledData.prevPressTime) return;
      digitalWrite(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, LED_ON);
      ledData.ledChainToggleButtonPressed = true;

      ledData.prevPressTime = millis() + BUTTON_MIN_DELAY + 100000L;
      ledData.flagWriteLEDCountToEEPROM = true;
    }
#else
  #define MAX_LED_COUNT DEFAULT_LED_COUNT
  CRGB leds[DEFAULT_LED_COUNT];
#endif

#include "functions/arrayFunctions.h"
#include "functions/randomBrightColour.h"
#include "functions/expandColour.h"

#include "patterns/colourSmash.h"
#include "patterns/cylon.h"                     // Depends on ledData
#include "patterns/cinematicRainbowFirework.h"  // Depends on ledData
#include "patterns/demoReel100.h"
#include "patterns/fallingSnow.h"
#include "patterns/fire2012.h"
#include "patterns/gradientChase.h"
#include "patterns/meteorRain.h"                // Depends on ledData
#include "patterns/moveRainbowLight.h"
#include "patterns/pride.h"
#include "patterns/rainbow2.h"
#include "patterns/rainbowChasers.h"
#include "patterns/rainbowFirework.h"           // Depends on ledData
#include "patterns/rainbowWaterfall.h"
#include "patterns/rainbowWaterfallHueRotate.h"
#include "patterns/rainbowWave.h"
#include "patterns/random.h"                    // Depends on ledData
#include "patterns/rotatingRainbowHue.h"
#include "patterns/rotatingRainbowHueSegmented.h"
#include "patterns/sinusoidalBeats.h"
#include "patterns/sinusoidalBeats2.h"
#include "patterns/sinusoidalBeatsWithBlur.h"
#include "patterns/staticFullChainRainbow.h"

void buttonPushEvent();

uint32_t brightColours[] = {
  0xff5555,  // Light Red
  0x00ff55,  // Bright Green
  0x00ff22,  // Vivid Green
  0x00aa00,  // Dark Green
  0x5555ff,  // Soft Blue
  0x3ce3b4,  // Turquoise
  0xffff00,  // Yellow
  0xff0000,  // Red
  0xffaa00,  // Orange
  0xff4400,  // Deep Orange
};
constexpr uint8_t brightColoursLen = sizeof(brightColours) / sizeof(brightColours[0]);

const uint16_t meteorColors[] = {
  0xF70,  // Orange
  0xFF0,  // Yellow
  0xF00,  // Red
  0xD15,  // Crimson
  0xFFF,  // White
  0xF55,  // Light Red
  0x0F4,  // Vivid Green
  0x33F,  // Soft Blue
  0x00F,  // Blue
  0x0F0,  // Green
  0x3EA,  // Teal
};
constexpr uint8_t meteorColoursLen = sizeof(meteorColors) / sizeof(meteorColors[0]);

void shutDown(uint16_t shutDownAnimDelay) {
  for (uint8_t i = 0; i < ledData.numLEDTotal; ++i) {
    leds[i] = (CRGB)0x0;
    FastLED.show();
    FastLED.delay(shutDownAnimDelay);
  }
}

/**
 * Displays the total number of LEDs on the LED strip as a series of white light indicators.
 * The number of indicators displayed is based on the total LED count, with each indicator
 * representing either 10 LEDs or the total count if less than 10. Each indicator LED
 * gradually brightens to full intensity before moving to the next.
 * After displaying the count, the last LED indicator flashes several times as a visual cue.
 *
 * @param numLEDTotal The total number of LEDs to be represented by the indicators.
 */
void showLEDCount(uint16_t numLEDTotal) {
  uint8_t indicators = numLEDTotal >= 10 ? numLEDTotal / 10 : numLEDTotal;

  // Fade in steps
  uint16_t stepFactor = indicators * 2;

  for (uint8_t i = 0; i < indicators; ++i) {
    leds[i] = CRGB::White;
    uint16_t step = 0;

    while (true) {
      if (step > 255) break;

      leds[i] = CRGB(step, step, step);

      FastLED.show();
      FastLED.delay(1);

      step += stepFactor;
    }
  }

  constexpr uint16_t flashDurationOnFactor = 1250;
  constexpr uint16_t flashDurationOffFactor = 625;

  for (uint8_t flash = 0; flash < indicators; ++flash) {
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

/**
 * Writes a given design pattern index to four specified EEPROM addresses for redundancy.
 * This ensures that even if one or more EEPROM locations become corrupted, the design pattern
 * index can still be recovered from the remaining locations, enhancing data reliability.
 *
 * @param design The design pattern index to be stored across the four EEPROM addresses.
 */
void writeDesignToEEPROM(uint8_t design) {
  EEPROM_SAVE_VALUE(EEPROM_DESIGN_ADDRESS_1, design);
  EEPROM_SAVE_VALUE(EEPROM_DESIGN_ADDRESS_2, design);
  EEPROM_SAVE_VALUE(EEPROM_DESIGN_ADDRESS_3, design);
  EEPROM_SAVE_VALUE(EEPROM_DESIGN_ADDRESS_4, design);
}

/**
 * Reads the design pattern index from four specified EEPROM addresses and determines
 * the most common value among them as the valid index. This method provides redundancy,
 * allowing recovery of the design pattern index even in the case of partial EEPROM corruption.
 * If no consensus on the index is found (indicating potential corruption), a default value
 * is returned, and the EEPROM locations are reset to this default.
 *
 * @return The most common valid design pattern index found, or a default value if no consensus exists.
 */
uint8_t readDesignFromEEPROM() {
  const uint8_t addresses[4] = {
    EEPROM_DESIGN_ADDRESS_1,
    EEPROM_DESIGN_ADDRESS_2,
    EEPROM_DESIGN_ADDRESS_3,
    EEPROM_DESIGN_ADDRESS_4
  };

  return readCommonValueFromEEPROM(addresses, 4, TOTAL_PATTERNS, 0);
}

/**
 * A generic function for reading and validating a value stored across multiple EEPROM addresses.
 * It checks for the most common value within a specified range across the given addresses,
 * employing redundancy to protect against data corruption. If a majority is found, that value is
 * considered valid and returned. Otherwise, the function resets all specified addresses to a
 * default value and returns this default, ensuring system resilience to EEPROM corruption.
 *
 * @param addresses Array of EEPROM addresses from which the value is read.
 * @param numAddresses Number of addresses to read, indicating the size of the array.
 * @param maxValue The maximum valid value expected, for validation purposes.
 * @param defaultValue The default value to reset to and return if no valid value is found.
 * @return The most common valid value across the addresses, or a default value in the absence of consensus.
 */
uint8_t readCommonValueFromEEPROM(const uint8_t addresses[], uint8_t numAddresses, uint8_t maxValue, uint16_t defaultValue) {
  uint8_t values[4];
  for (uint8_t i = 0; i < numAddresses; i++) {
    values[i] = EEPROM.read(addresses[i]);
  }

  uint8_t finalValue = 0; // Default value if none match
  bool validFound = false;

  for (uint8_t i = 0; i < numAddresses; i++) {

    if (values[i] <= maxValue) {
      uint8_t matchCount = 1; // Start with a count of 1 for the current value

      // Compare with remaining values
      for (uint8_t j = i + 1; j < numAddresses; j++) {
        if (values[i] == values[j]) {
          matchCount++;
        }
      }

      // If this value has a majority or matches at least one other (considering possible corruption),
      // and no valid value has been found yet or a better match is found (more occurrences)
      if ((matchCount >= 2) && (!validFound || matchCount > 2)) {
        finalValue = values[i];
        validFound = true;
        // If we found a value with 3 matches, it's the best scenario, no need to continue
        if (matchCount > 2) break;
      }
    }
  }

  // If no majority found, reset all to default
  if (!validFound) {
    for (uint8_t i = 0; i < numAddresses; i++) {
      EEPROM_SAVE_VALUE(addresses[i], defaultValue); // Reset all to default
    }
    return 0; // Default value
  } else {
    return finalValue; // Return the most common valid value
  }
}

// Use static inline to allow the compiler to inline it fully for speed and no call overhead
static inline void showSolid(CRGB *ledArray, uint16_t count, uint16_t colour) {
  uint32_t expandedColour = expandColour(colour);
  fill_solid(ledArray, count, CRGB(expandedColour));
  FastLED.show();
}

void setup() {
  delay(500);

  srand(analogRead(A0));
  // Serial.begin(115200);

  #ifdef BOARD_ESP8266
    EEPROM.begin(128);
  #endif

  pinMode(BUTTON_DESIGN_LED_INDICATOR_PIN, OUTPUT);
  digitalWrite(BUTTON_DESIGN_LED_INDICATOR_PIN, LED_OFF);

  // Button press event
  pinMode(DESIGN_BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(DESIGN_BUTTON_PIN), buttonPushEvent, RISING);

  #ifdef LED_CHAIN_TOGGLE_BUTTON
    pinMode(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, OUTPUT);
    digitalWrite(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, LED_OFF);

    pinMode(LED_CHAIN_TOGGLE_BUTTON_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(LED_CHAIN_TOGGLE_BUTTON_PIN), resetButtonPushEvent, RISING);

    ledData.numLEDTotal = readLEDCountFromEEPROM();

    // If the stored value is not within our expected range, reset it to DEFAULT_LED_COUNT
    if (ledData.numLEDTotal < DEFAULT_LED_COUNT || ledData.numLEDTotal > MAX_LED_COUNT || ledData.numLEDTotal % DEFAULT_LED_COUNT != 0) {
      ledData.numLEDTotal = DEFAULT_LED_COUNT;
      writeLEDCountToEEPROM(ledData.numLEDTotal);
    }
  #else
    ledData.numLEDTotal = DEFAULT_LED_COUNT;
  #endif

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, ledData.numLEDTotal)
    .setCorrection(TypicalLEDStrip)
    .setDither(false);

  FastLED.setBrightness(BRIGHTNESS);

  FastLED.clear();

  #ifdef LED_CHAIN_TOGGLE_BUTTON
    showLEDCount(ledData.numLEDTotal);
  #endif

  ledData.design = readDesignFromEEPROM();
  if (ledData.design > TOTAL_PATTERNS) {
    ledData.design = 0;
    writeDesignToEEPROM(ledData.design);
  }
}

void loop() {
  const uint8_t selectedDesign = ledData.design;
  const uint8_t totalLEDCount = ledData.numLEDTotal;
  const uint32_t currentTime = millis();

  #ifdef LED_CHAIN_TOGGLE_BUTTON
    if (ledData.flagWriteLEDCountToEEPROM) {
      ledData.flagWriteLEDCountToEEPROM = false;

      delay(1000);
      updateLEDCount();

      digitalWrite(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, LED_ON);
      delay(50);

      restartMCU();

      return;
    }
  #endif

  // Restart after ~48 days (to be safe)
  if (currentTime > MILLIS_OVERFLOW_RESTART_THRESHOLD) {
    restartMCU();
  }

  if (ledData.modeButtonPressed && ledData.modeButtonActivatedTill < currentTime) {
    ledData.modeButtonPressed = false;
    digitalWrite(BUTTON_DESIGN_LED_INDICATOR_PIN, LED_OFF);
  }

  if (!ledData.hasUpdatedOnButtonPress) {
    ledData.hasUpdatedOnButtonPress = true;
  }

  if (ledData.designChangeDetected) {
    ledData.designChangeDetected = false;
    ledData.eepromWritePendingDelay = true;
    ledData.scheduledEEPROMWriteTime = currentTime + EEPROM_WRITE_DELAY_AFTER_LAST_CHANGE;
  }

  if (ledData.eepromWritePendingDelay && currentTime > ledData.scheduledEEPROMWriteTime) {
    ledData.eepromWritePendingDelay = false;
    writeDesignToEEPROM(ledData.design);
  }

  if (selectedDesign == 0) {
    rotatingRainbowHue(totalLEDCount);
    FastLED.show();
    FastLED.delay(5);
  } else if (selectedDesign == 1) {
    rotatingRainbowHueSegmented(totalLEDCount, totalLEDCount / 4);
    FastLED.show();
    FastLED.delay(5);
  } else if (selectedDesign == 2) {
    rotatingRainbowHue(totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.show();
    FastLED.delay(5);
  } else if (selectedDesign == 3) {
    rainbowFirework(totalLEDCount);
  } else if (selectedDesign == 4) {
    staticRainbowChase(leds, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 5) {
    dynamicRainbowChase(leds, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 6) {
    cinematicRainbowFirework(totalLEDCount);
  } else if (selectedDesign == 7) {
    rainbowWaterfall(30, 7, totalLEDCount);
    FastLED.show();
    FastLED.delay(30);
  } else if (selectedDesign == 8) {
    rainbowWaterfallHueRotate(30, 7, totalLEDCount);
    FastLED.show();
    FastLED.delay(30);
  } else if (selectedDesign == 9) {
    reverseRainbowWaterfall(30, 7, totalLEDCount);
    FastLED.show();
    FastLED.delay(30);
  } else if (selectedDesign == 10) {
    reverseRainbowWaterfallHueRotate(30, 7, totalLEDCount);
    FastLED.show();
    FastLED.delay(30);
  } else if (selectedDesign == 11) {
    fallingRainbowSnowEffect(leds, totalLEDCount);
  } else if (selectedDesign == 12) {
    gradientChase(leds, totalLEDCount, false);
  } else if (selectedDesign == 13) {
    gradientChase(leds, totalLEDCount, true);
  } else if (selectedDesign == 14) {
    gradientChaseClash(leds, totalLEDCount);
  } else if (selectedDesign == 15) {
    moveRainbowLight(totalLEDCount, leds);
  } else if (selectedDesign == 16) {
    staticFullChainRainbow(leds, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 17) {
    staticFullChainRainbow(leds, totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 18) {
    rainbowWave(7, totalLEDCount);
  } else if (selectedDesign == 19) {
    fill_rainbow(leds, totalLEDCount, 0, 7);
    FastLED.show();
  } else if (selectedDesign == 20) {
    fill_rainbow(leds, totalLEDCount, 0, 7);
    addGlitter(50, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 21) {
    rainbow2(totalLEDCount, 6);
    FastLED.show();
  } else if (selectedDesign == 22) {
    colourSmash(totalLEDCount);
    FastLED.show();
    FastLED.delay(20);
  } else if (selectedDesign == 23) {
    colourSmash2(totalLEDCount);
    FastLED.show();
    FastLED.delay(20);
  } else if (selectedDesign == 24) {
    colourSmash(totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.show();
    FastLED.delay(20);
  } else if (selectedDesign == 25) {
    colourSmash2(totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.show();
    FastLED.delay(20);
  } else if (selectedDesign == 26) {
    sinusoidalBeats(totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 27) {
    sinusoidalBeats(totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 28) {
    sinusoidalBeatsWithBlur(totalLEDCount);
  } else if (selectedDesign == 29) {
    sinusoidalBeats2(totalLEDCount);
  } else if (selectedDesign == 30) {
    rotateBrightColoursSmooth(brightColours, brightColoursLen, totalLEDCount);
  } else if (selectedDesign == 31 || selectedDesign == 32 || selectedDesign == 33) {
    if (!ledData.randomColoursSet) {
      ledData.randomColoursSet = true;
      arrayShuffleUniqColoursEffect(brightColours, brightColoursLen, leds, totalLEDCount);
    }
    FastLED.show();
  } else if (selectedDesign == 34) {
    uint8_t meteorColourIndex = random8(meteorColoursLen);
    uint32_t meteorColour = expandColour(meteorColors[meteorColourIndex]);
    meteorRain(meteorColour, totalLEDCount);
  } else if (selectedDesign == 35) {
    pride(totalLEDCount);
  } else if (selectedDesign == 36) {
    cylon(totalLEDCount);
  } else if (selectedDesign == 37) {
    cylon2(totalLEDCount);
  } else if (selectedDesign == 38) {
    demoReel100(totalLEDCount);
  } else if (selectedDesign == 39) {
    fire2012(totalLEDCount);
  } else if (selectedDesign == 40) {
    showSolid(leds,totalLEDCount, 0xf51);
  } else if (selectedDesign == 41) {
    showSolid(leds,totalLEDCount, 0x088);
  } else if (selectedDesign == 42) {
    showSolid(leds,totalLEDCount, 0x3c3);
  } else if (selectedDesign == 43) {
    showSolid(leds,totalLEDCount, 0x0ff);
  } else if (selectedDesign == 44) {
    showSolid(leds,totalLEDCount, 0xf5d);
  } else if (selectedDesign == 45) {
    showSolid(leds,totalLEDCount, 0xf0f);
  } else if (selectedDesign == 46) {
    showSolid(leds,totalLEDCount, 0xff0);
  } else if (selectedDesign == 47) {
    showSolid(leds,totalLEDCount, 0xf20);
  } else if (selectedDesign == 48) {
    showSolid(leds,totalLEDCount, 0x0f0);
  } else if (selectedDesign == 49) {
    showSolid(leds,totalLEDCount, 0x3eb);
  } else if (selectedDesign == 50) {
    showSolid(leds,totalLEDCount, 0xf01);
  } else if (selectedDesign == 51) {
    rotateBrightColours(brightColours, brightColoursLen, totalLEDCount);
  } else {
    shutDown(10);
  }
}

#ifdef BOARD_ESP8266
  void IRAM_ATTR buttonPushEvent() {
#else
  void buttonPushEvent() {
#endif
    // Prevent skipping effects
    if (!ledData.hasUpdatedOnButtonPress) return;

    const uint32_t currentTime = millis();

    // Minimum delay between switch presses (sharing all the buttons)
    if (currentTime < ledData.prevPressTime) return;

    ledData.prevPressTime = currentTime + BUTTON_MIN_DELAY;
    ledData.design = (ledData.design + 1) % (TOTAL_PATTERNS + 2);

    digitalWrite(BUTTON_DESIGN_LED_INDICATOR_PIN, LED_ON);
    ledData.modeButtonPressed = true;
    ledData.modeButtonActivatedTill = currentTime + 500;
    ledData.hasUpdatedOnButtonPress = false;

    // Indicate a design change has been detected, triggering the preparation for an EEPROM write.
    ledData.designChangeDetected = true;

    ledData.randomColoursSet = false;
    colourSmashData.colourSmashInitialized = false;
  }
