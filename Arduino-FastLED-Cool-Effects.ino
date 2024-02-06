#pragma GCC optimize("Os")

#include <Arduino.h>
#include <FastLED.h>
#include <EEPROM.h>

#define TOTAL_PATTERNS 46

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
#ifdef LED_ACTIVE_LOW
  #define LED_ON LOW
  #define LED_OFF HIGH
#else
  #define LED_ON HIGH
  #define LED_OFF LOW
#endif

struct LEDData {
  volatile unsigned long long modeButtonActivatedTill = 0;
  volatile unsigned long prevPressTime = 0;
  int numLEDTotal = DEFAULT_LED_COUNT;
  uint8_t design = 0;
  uint8_t brightness = 255;
  volatile bool hasUpdatedOnButtonPress = false;
  volatile bool randomColoursSet = false;
  volatile bool modeButtonPressed = false;
};

LEDData ledData;

#ifdef LED_CHAIN_TOGGLE_BUTTON
  volatile bool resetButtonPressed = false;
  CRGB leds[MAX_LED_COUNT];

  //declare reset function at address 0
  void (*resetFunc)(void) = 0;

  void writeLEDCountToEEPROM(uint16_t num) {
    EEPROM.update(EEPROM_LED_COUNT_ADDRESS_1, num);
    EEPROM.update(EEPROM_LED_COUNT_ADDRESS_2, num);
    EEPROM.update(EEPROM_LED_COUNT_ADDRESS_3, num);
    EEPROM.update(EEPROM_LED_COUNT_ADDRESS_4, num);
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

  void resetButtonPushEvent() {
    // Minimum delay between switch presses (sharing all the buttons)
    if (millis() < ledData.prevPressTime) return;
    digitalWrite(CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN, LED_ON);

    ledData.prevPressTime = millis() + BUTTON_MIN_DELAY + 100000;
    resetButtonPressed = true;
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

void buttonPushEvent();

unsigned long brightColours[] = {
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
const unsigned char brightColoursLen = sizeof(brightColours) / sizeof(brightColours[0]);

unsigned long meteorColors[] = {
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
const unsigned char meteorColoursLen = sizeof(meteorColors) / sizeof(meteorColors[0]);

void shutDown(int shutDownAnimDelay) {
  for (unsigned char i = 0; i < ledData.numLEDTotal; ++i) {
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
void showLEDCount(unsigned short numLEDTotal) {
  unsigned char indicators = numLEDTotal >= 10 ? numLEDTotal / 10 : numLEDTotal;

  // Fade in steps
  unsigned short stepFactor = indicators * 2;

  for (int i = 0; i < indicators; ++i) {
    leds[i] = CRGB::White;

    unsigned short step = 0;

    while (true) {
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

/**
 * Writes a given design pattern index to four specified EEPROM addresses for redundancy.
 * This ensures that even if one or more EEPROM locations become corrupted, the design pattern
 * index can still be recovered from the remaining locations, enhancing data reliability.
 *
 * @param design The design pattern index to be stored across the four EEPROM addresses.
 */
void writeDesignToEEPROM(uint8_t design) {
  EEPROM.update(EEPROM_DESIGN_ADDRESS_1, design);
  EEPROM.update(EEPROM_DESIGN_ADDRESS_2, design);
  EEPROM.update(EEPROM_DESIGN_ADDRESS_3, design);
  EEPROM.update(EEPROM_DESIGN_ADDRESS_4, design);
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
      EEPROM.update(addresses[i], defaultValue); // Reset all to default
    }
    return 0; // Default value
  } else {
    return finalValue; // Return the most common valid value
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

  if (ledData.modeButtonPressed && ledData.modeButtonActivatedTill < millis()) {
    ledData.modeButtonPressed = false;
    digitalWrite(BUTTON_PUSH_LED_INDICATOR_PIN, LED_OFF);
  }

  if (!ledData.hasUpdatedOnButtonPress) {
    ledData.hasUpdatedOnButtonPress = true;
  }

  if (ledData.design == 0) {
    rotatingRainbowHue(ledData.numLEDTotal);
    FastLED.delay(5);
    FastLED.show();
  } else if (ledData.design == 1) {
    rotatingRainbowHue(ledData.numLEDTotal);
    addGlitter(50, ledData.numLEDTotal);
    FastLED.delay(5);
    FastLED.show();
  } else if (ledData.design == 2) {
    rainbowFirework(ledData.numLEDTotal);
  } else if (ledData.design == 3) {
    staticRainbowChase(leds, ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 4) {
    dynamicRainbowChase(leds, ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 5) {
    rainbowWaterfall(30, 7, ledData.numLEDTotal);
    FastLED.show();
    FastLED.delay(30);
  } else if (ledData.design == 6) {
    rainbowWaterfallHueRotate(30, 7, ledData.numLEDTotal);
    FastLED.show();
    FastLED.delay(30);
  } else if (ledData.design == 7) {
    reverseRainbowWaterfall(30, 7, ledData.numLEDTotal);
    FastLED.show();
    FastLED.delay(30);
  } else if (ledData.design == 8) {
    reverseRainbowWaterfallHueRotate(30, 7, ledData.numLEDTotal);
    FastLED.show();
    FastLED.delay(30);
  } else if (ledData.design == 9) {
    moveRainbowLight(ledData.numLEDTotal, leds);
  } else if (ledData.design == 10) {
    staticFullChainRainbow(leds, ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 11) {
    staticFullChainRainbow(leds, ledData.numLEDTotal);
    addGlitter(50, ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 12) {
    rainbowWave(10, ledData.numLEDTotal);
  } else if (ledData.design == 13) {
    fill_rainbow(leds, ledData.numLEDTotal, 0, 7);
    FastLED.show();
  } else if (ledData.design == 14) {
    fill_rainbow(leds, ledData.numLEDTotal, 0, 7);
    addGlitter(50, ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 15) {
    rainbow2(ledData.numLEDTotal, 6);
    FastLED.show();
  } else if (ledData.design == 16) {
    rainbowSmash(ledData.numLEDTotal);
    FastLED.show();
    FastLED.delay(15);
  } else if (ledData.design == 17) {
    colourSmash(ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 18) {
    colourSmash2(ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 19) {
    colourSmash(ledData.numLEDTotal);
    addGlitter(50, ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 20) {
    colourSmash2(ledData.numLEDTotal);
    addGlitter(50, ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 21) {
    sinusoidalBeats(ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 22) {
    sinusoidalBeats(ledData.numLEDTotal);
    addGlitter(50, ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 23) {
    sinusoidalBeatsWithBlur(ledData.numLEDTotal);
  } else if (ledData.design == 24) {
    sinusoidalBeats2(ledData.numLEDTotal);
  } else if (ledData.design == 25) {
    radiantShimmer(ledData.numLEDTotal);
    FastLED.show();
  } else if (ledData.design == 26) {
    rotateBrightColoursSmooth(brightColours, brightColoursLen, ledData.numLEDTotal);
  } else if (ledData.design == 27 || ledData.design == 28 || ledData.design == 29) {
    if (!ledData.randomColoursSet) {
      ledData.randomColoursSet = true;
      arrayShuffleUniqColoursEffect(brightColours, brightColoursLen, leds, ledData.numLEDTotal);
    }
    FastLED.show();
  } else if (ledData.design == 30) {
    int meteorColourIndex = rand() % meteorColoursLen;
    uint32_t meteorColour = meteorColors[meteorColourIndex];
    meteorRain(meteorColour, ledData.numLEDTotal);
  } else if (ledData.design == 31) {
    pride(ledData.numLEDTotal);
  } else if (ledData.design == 32) {
    cylon(ledData.numLEDTotal);
  } else if (ledData.design == 33) {
    demoReel100(ledData.numLEDTotal);
  } else if (ledData.design == 34) {
    fire2012(ledData.numLEDTotal);
  } else if (ledData.design == 35) {
    fill_solid(leds, ledData.numLEDTotal, 0xff5010);
    FastLED.show();
  } else if (ledData.design == 36) {
    fill_solid(leds, ledData.numLEDTotal, 0x008080);
    FastLED.show();
  } else if (ledData.design == 37) {
    fill_solid(leds, ledData.numLEDTotal, 0x32cd32);
    FastLED.show();
  } else if (ledData.design == 38) {
    fill_solid(leds, ledData.numLEDTotal, 0x00ffff);
    FastLED.show();
  } else if (ledData.design == 39) {
    fill_solid(leds, ledData.numLEDTotal, 0xff50d6);
    FastLED.show();
  } else if (ledData.design == 40) {
    fill_solid(leds, ledData.numLEDTotal, 0xff00ff);
    FastLED.show();
  } else if (ledData.design == 41) {
    fill_solid(leds, ledData.numLEDTotal, 0xffff00);
    FastLED.show();
  } else if (ledData.design == 42) {
    fill_solid(leds, ledData.numLEDTotal, 0xff2200);
    FastLED.show();
  } else if (ledData.design == 43) {
    fill_solid(leds, ledData.numLEDTotal, 0x00ff00);
    FastLED.show();
  } else if (ledData.design == 44) {
    fill_solid(leds, ledData.numLEDTotal, 0x3ce3b4);
    FastLED.show();
  } else if (ledData.design == 45) {
    fill_solid(leds, ledData.numLEDTotal, 0xff0011);
    FastLED.show();
  } else if (ledData.design == 46) {
    rotateBrightColours(brightColours, brightColoursLen, ledData.numLEDTotal);
  } else {
    shutDown(10);
  }
}

void buttonPushEvent() {
  // Prevent skipping effects
  if (!ledData.hasUpdatedOnButtonPress) return;

  // Minimum delay between switch presses (sharing all the buttons)
  if (millis() < ledData.prevPressTime) return;

  ledData.prevPressTime = millis() + BUTTON_MIN_DELAY;
  ledData.design = (ledData.design + 1) % (TOTAL_PATTERNS + 2);
  writeDesignToEEPROM(ledData.design);

  digitalWrite(BUTTON_PUSH_LED_INDICATOR_PIN, LED_ON);
  ledData.modeButtonPressed = true;
  ledData.modeButtonActivatedTill = millis() + 500;
  ledData.hasUpdatedOnButtonPress = false;

  ledData.randomColoursSet = false;
  colourSmashData.colourSmashInitialized = false;

  FastLED.setBrightness(BRIGHTNESS);
}
