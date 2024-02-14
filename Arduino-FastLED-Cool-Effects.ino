#pragma GCC optimize("Os")

#define TOTAL_PATTERNS 48

#include <Arduino.h>
#include <EEPROM.h>
#include "board.h"

#if !defined(BOARD_ESP8266) && !defined(BOARD_ARDUINO)
  #warning "Neither BOARD_ESP8266 nor BOARD_ARDUINO is defined. Using Arduino by default"
  #define BOARD_ARDUINO
#endif

#include <FastLED.h>

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
  uint16_t numLEDTotal = DEFAULT_LED_COUNT;
  uint8_t design = 0;
  bool hasUpdatedOnButtonPress = false;
  bool randomColoursSet = false;
  bool modeButtonPressed = false;
  bool designChangeDetected = false;
  bool eepromWritePendingDelay = false;
  bool flagWriteLEDCountToEEPROM = false;
} ledData;

#ifdef LED_CHAIN_TOGGLE_BUTTON
  CRGB leds[MAX_LED_COUNT];

  //declare reset function at address 0
  void (*resetFunc)(void) = 0;

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
    ledData.numLEDTotal += DEFAULT_LED_COUNT;

    if (ledData.numLEDTotal > MAX_LED_COUNT) {
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

      ledData.prevPressTime = millis() + BUTTON_MIN_DELAY + 100000L;
      ledData.flagWriteLEDCountToEEPROM = true;
    }
#else
  #define MAX_LED_COUNT DEFAULT_LED_COUNT
  CRGB leds[DEFAULT_LED_COUNT];
#endif

#include "functions/arrayFunctions.h"
#include "functions/randomBrightColour.h"

#include "patterns/colourSmash.h"
#include "patterns/cylon.h"
#include "patterns/demoReel100.h"
#include "patterns/fallingSnow.h"
#include "patterns/fire2012.h"
#include "patterns/gradientChase.h"
#include "patterns/meteorRain.h"
#include "patterns/moveRainbowLight.h"
#include "patterns/pride.h"
#include "patterns/rainbow2.h"
#include "patterns/rainbowChasers.h"
#include "patterns/rainbowFirework.h"
#include "patterns/rainbowWaterfall.h"
#include "patterns/rainbowWaterfallHueRotate.h"
#include "patterns/rainbowWave.h"
#include "patterns/random.h"
#include "patterns/rotatingRainbowHue.h"
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
const uint8_t brightColoursLen = sizeof(brightColours) / sizeof(brightColours[0]);

uint32_t meteorColors[] = {
  0xFF4500,  // Orange Red
  0xFFD700,  // Gold
  0xFF8C00,  // Dark Orange
  0xFFA500,  // Orange
  0xFFFF00,  // Yellow
  0xFF6347,  // Tomato
  0xFF0000,  // Red
  0xDC143C,  // Crimson
  0xB22222,  // Fire Brick
  0x8B0000,  // Dark Red
  0x800000,  // Maroon
  0xDAA520,  // Golden Rod
  0xCD5C5C,  // Indian Red
  0xF08080,  // Light Coral
  0xE9967A,  // Dark Salmon
  0xFA8072,  // Salmon
  0xFFA07A,  // Light Salmon
  0xFF7F50   // Coral
};
const uint8_t meteorColoursLen = sizeof(meteorColors) / sizeof(meteorColors[0]);

void shutDown(uint16_t shutDownAnimDelay) {
  for (uint16_t i = 0; i < ledData.numLEDTotal; ++i) {
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

  uint16_t flashDurationOnFactor = 1250;
  uint16_t flashDurationOffFactor = 625;

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
  uint8_t values[4] = {0};
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

void setup() {
  delay(1000);

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
    .setDither(BRIGHTNESS < 255);

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
  uint8_t selectedDesign = ledData.design;
  uint8_t totalLEDCount = ledData.numLEDTotal;
  uint32_t currentTime = millis();

  #ifdef LED_CHAIN_TOGGLE_BUTTON
    if (ledData.flagWriteLEDCountToEEPROM) {
      ledData.flagWriteLEDCountToEEPROM = false;

      delay(1000);
      updateLEDCount();

      digitalWrite(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, LED_ON);
      delay(50);

      #if defined(BOARD_ESP8266)
        ESP.restart();
      #elif defined(BOARD_ARDUINO)
        resetFunc();
      #endif

      return;
    }
  #endif

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
    FastLED.delay(5);
    FastLED.show();
  } else if (selectedDesign == 1) {
    rotatingRainbowHue(totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.delay(5);
    FastLED.show();
  } else if (selectedDesign == 2) {
    rainbowFirework(totalLEDCount);
  } else if (selectedDesign == 3) {
    staticRainbowChase(leds, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 4) {
    dynamicRainbowChase(leds, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 5) {
    rainbowWaterfall(30, 7, totalLEDCount);
    FastLED.show();
    FastLED.delay(30);
  } else if (selectedDesign == 6) {
    rainbowWaterfallHueRotate(30, 7, totalLEDCount);
    FastLED.show();
    FastLED.delay(30);
  } else if (selectedDesign == 7) {
    reverseRainbowWaterfall(30, 7, totalLEDCount);
    FastLED.show();
    FastLED.delay(30);
  } else if (selectedDesign == 8) {
    reverseRainbowWaterfallHueRotate(30, 7, totalLEDCount);
    FastLED.show();
    FastLED.delay(30);
  } else if (selectedDesign == 9) {
    fallingRainbowSnowEffect(leds, totalLEDCount);
  } else if (selectedDesign == 10) {
    gradientChase(leds, totalLEDCount, false);
  } else if (selectedDesign == 11) {
    gradientChase(leds, totalLEDCount, true);
  } else if (selectedDesign == 12) {
    gradientChaseClash(leds, totalLEDCount);
  } else if (selectedDesign == 13) {
    moveRainbowLight(totalLEDCount, leds);
  } else if (selectedDesign == 14) {
    staticFullChainRainbow(leds, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 15) {
    staticFullChainRainbow(leds, totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 16) {
    rainbowWave(7, totalLEDCount);
  } else if (selectedDesign == 17) {
    fill_rainbow(leds, totalLEDCount, 0, 7);
    FastLED.show();
  } else if (selectedDesign == 18) {
    fill_rainbow(leds, totalLEDCount, 0, 7);
    addGlitter(50, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 19) {
    rainbow2(totalLEDCount, 6);
    FastLED.show();
  } else if (selectedDesign == 20) {
    colourSmash(totalLEDCount);
    FastLED.show();
    FastLED.delay(20);
  } else if (selectedDesign == 21) {
    colourSmash2(totalLEDCount);
    FastLED.show();
    FastLED.delay(20);
  } else if (selectedDesign == 22) {
    colourSmash(totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.show();
    FastLED.delay(20);
  } else if (selectedDesign == 23) {
    colourSmash2(totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.show();
    FastLED.delay(20);
  } else if (selectedDesign == 24) {
    sinusoidalBeats(totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 25) {
    sinusoidalBeats(totalLEDCount);
    addGlitter(50, totalLEDCount);
    FastLED.show();
  } else if (selectedDesign == 26) {
    sinusoidalBeatsWithBlur(totalLEDCount);
  } else if (selectedDesign == 27) {
    sinusoidalBeats2(totalLEDCount);
  } else if (selectedDesign == 28) {
    rotateBrightColoursSmooth(brightColours, brightColoursLen, totalLEDCount);
  } else if (selectedDesign == 29 || selectedDesign == 30 || selectedDesign == 31) {
    if (!ledData.randomColoursSet) {
      ledData.randomColoursSet = true;
      arrayShuffleUniqColoursEffect(brightColours, brightColoursLen, leds, totalLEDCount);
    }
    FastLED.show();
  } else if (selectedDesign == 32) {
    int16_t meteorColourIndex = rand() % meteorColoursLen;
    uint32_t meteorColour = meteorColors[meteorColourIndex];
    meteorRain(meteorColour, totalLEDCount);
  } else if (selectedDesign == 33) {
    pride(totalLEDCount);
  } else if (selectedDesign == 34) {
    cylon(totalLEDCount);
  } else if (selectedDesign == 35) {
    demoReel100(totalLEDCount);
  } else if (selectedDesign == 36) {
    fire2012(totalLEDCount);
  } else if (selectedDesign == 37) {
    fill_solid(leds, totalLEDCount, 0xff5010);
    FastLED.show();
  } else if (selectedDesign == 38) {
    fill_solid(leds, totalLEDCount, 0x008080);
    FastLED.show();
  } else if (selectedDesign == 39) {
    fill_solid(leds, totalLEDCount, 0x32cd32);
    FastLED.show();
  } else if (selectedDesign == 40) {
    fill_solid(leds, totalLEDCount, 0x00ffff);
    FastLED.show();
  } else if (selectedDesign == 41) {
    fill_solid(leds, totalLEDCount, 0xff50d6);
    FastLED.show();
  } else if (selectedDesign == 42) {
    fill_solid(leds, totalLEDCount, 0xff00ff);
    FastLED.show();
  } else if (selectedDesign == 43) {
    fill_solid(leds, totalLEDCount, 0xffff00);
    FastLED.show();
  } else if (selectedDesign == 44) {
    fill_solid(leds, totalLEDCount, 0xff2200);
    FastLED.show();
  } else if (selectedDesign == 45) {
    fill_solid(leds, totalLEDCount, 0x00ff00);
    FastLED.show();
  } else if (selectedDesign == 46) {
    fill_solid(leds, totalLEDCount, 0x3ce3b4);
    FastLED.show();
  } else if (selectedDesign == 47) {
    fill_solid(leds, totalLEDCount, 0xff0011);
    FastLED.show();
  } else if (selectedDesign == 48) {
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

    uint32_t currentTime = millis();

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
