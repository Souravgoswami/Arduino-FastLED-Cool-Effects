struct RainbowFireworkData {
  const uint16_t delay = 2500; // The delay time in milliseconds to hold the rainbow
  uint32_t releaseDelayAt = 0;
  bool inDelayedPeriod = false;
};

// Global instance of the structure to hold the animation state and timing.
RainbowFireworkData rainbowFireworkData;

void rainbowFirework(uint8_t numLEDs) {
  // Check if the animation is currently in its delay period.
  if (rainbowFireworkData.inDelayedPeriod) {
    if (millis() > rainbowFireworkData.releaseDelayAt) {
      rainbowFireworkData.inDelayedPeriod = false;
    } else {
      // If still in the delay period, update the LEDs (if needed) and return early.
      FastLED.show();
      return;
    }
  }

  // Main animation logic.
  // Randomly select a starting LED for the animation.
  uint8_t startLed = random8(numLEDs);

  // Define minimum and maximum delay factors for the animation effects.
  uint16_t delayFactorMin = 750; // 7.5ms for 1 LED, 1250 for 100
  uint16_t delayFactorMax = 1250; // 12.5ms for 1 LED, 1250 for 100
  uint16_t delayPerLEDMin = delayFactorMin / numLEDs;
  uint16_t delayPerLEDMax = delayFactorMax / numLEDs;

  // Gradually fade all LEDs to black.
  for (uint16_t i = 0; i < 256; i++) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    fadeToBlackBy(leds, numLEDs, 1);
    FastLED.show();
    FastLED.delay(5);
  }

  // Clear all LEDs and then immediately update the display.
  FastLED.clear();
  FastLED.show();
  FastLED.delay(5);

  // Set the color of the randomly selected start LED to a hue.
  uint8_t startHue = random(256);
  leds[startLed] = CHSV(startHue, 255, 255);
  FastLED.show();

  // Pause for a random duration between animations.
  uint16_t randomDelayDuration = random16(250, 500);
  FastLED.delay(randomDelayDuration);

  // Create a fizzling effect on the start LED.
  uint16_t flashDuration = random8(10, 25); // Duration for the fizzling effect.
  for(uint16_t i = 0; i < flashDuration; ++i) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    // Randomly alternate the color of the start LED between white and its hue.
    leds[startLed] = (random8(10) >= 5) ? CRGB::White : CRGB(CHSV(startHue, 255, 255));
    FastLED.show();
    FastLED.delay(random8(20, 60)); // Random delay for the flickering effect.
  }

  // Create a rapidly increasing flash rate effect on the start LED.
  uint16_t rapidFlashDuration = random8(10, 20); // Duration for the rapid flashing.
  for (uint16_t i = 0; i < rapidFlashDuration; ++i) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    leds[startLed] = (i % 2 == 0) ? CRGB::White : CRGB(CHSV(startHue, 255, 255));
    FastLED.show();
    FastLED.delay(i);
  }

  // Restore the start LED's color.
  leds[startLed] = CHSV(startHue, 255, 255);
  FastLED.show();
  FastLED.delay(random8(20, 60)); // Random delay before moving on.

  // Initialize an array to track the brightness level of each LED.
  uint8_t ledBrightness[numLEDs] = {0};

  // Gradually increase the brightness of each LED to create a rainbow effect.
  for (uint8_t offset = 1; offset < numLEDs; offset++) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    for (uint8_t i = 0; i < numLEDs; i++) {
      if (i == startLed) continue; // Skip the start LED.

      // Calculate the distance from the current LED to the start LED.
      uint16_t distance = abs(static_cast<int16_t>(i) - static_cast<int16_t>(startLed));

      // If the current LED is within the offset, adjust its brightness and color.
      if (distance <= offset) {
        int16_t hueOffset = distance * 5; // Determine the color spread in the rainbow.
        uint8_t hue = startHue + hueOffset;
        ledBrightness[i] = min(ledBrightness[i] + 20, 255);
        leds[i] = CHSV(hue, 255, ledBrightness[i]);
      }
    }

    // Show the updated LEDs and delay before the next iteration.
    FastLED.show();
    randomDelayDuration = random8(delayPerLEDMin, delayPerLEDMax);
    FastLED.delay(randomDelayDuration);
  }

  // Final brightness increase step for all LEDs.
  for (int16_t step = 0; step < 255; step += 5) {
    bool updateNeeded = false;
    for (uint8_t i = 0; i < numLEDs; i++) {
      if (i == startLed) continue; // Skip the start LED.

      // Increase brightness only if not already at maximum.
      if (ledBrightness[i] < 255) {
        ledBrightness[i] = min(ledBrightness[i] + 5, 255);
        uint16_t distance = abs(static_cast<int16_t>(i) - static_cast<int16_t>(startLed));
        uint16_t hueOffset = distance * 5; // Determine the color spread in the rainbow.
        uint8_t hue = startHue + hueOffset;
        leds[i] = CHSV(hue, 255, ledBrightness[i]);
        updateNeeded = true;
      }
    }

    // Show the updates only if needed.
    if (updateNeeded) {
      FastLED.show();
    }
  }

  // Set the flag to start the non-blocking delay period and calculate the release time.
  rainbowFireworkData.inDelayedPeriod = true;
  rainbowFireworkData.releaseDelayAt = millis() + rainbowFireworkData.delay;
}
