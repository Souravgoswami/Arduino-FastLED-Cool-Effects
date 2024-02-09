struct RainbowFireworkData {
  unsigned short delay = 2500; // The delay time in milliseconds to hold the rainbow
  unsigned long releaseDelayAt = 0;
  bool inDelayedPeriod = false;
};

// Global instance of the structure to hold the animation state and timing.
RainbowFireworkData rainbowFireworkData;

void rainbowFirework(unsigned short numLEDs) {
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
  unsigned char startLed = random(numLEDs);

  // Define minimum and maximum delay factors for the animation effects.
  unsigned int delayFactorMin = 750; // 7.5ms for 1 LED, 1250 for 100
  unsigned int delayFactorMax = 1250; // 12.5ms for 1 LED, 1250 for 100
  unsigned int delayPerLEDMin = delayFactorMin / numLEDs;
  unsigned int delayPerLEDMax = delayFactorMax / numLEDs;

  // Gradually fade all LEDs to black.
  for (unsigned short i = 0; i < 256; i++) {
    fadeToBlackBy(leds, numLEDs, 1);
    FastLED.show();
    FastLED.delay(5);
  }

  // Clear all LEDs and then immediately update the display.
  FastLED.clear();
  FastLED.show();
  FastLED.delay(5);

  // Set the color of the randomly selected start LED to a hue.
  unsigned char startHue = random(256);
  leds[startLed] = CHSV(startHue, 255, 255);
  FastLED.show();

  // Pause for a random duration between animations.
  unsigned short randomDelayDuration = random16(250, 500);
  FastLED.delay(randomDelayDuration);

  // Create a fizzling effect on the start LED.
  unsigned short flashDuration = random8(10, 25); // Duration for the fizzling effect.
  for(unsigned short i = 0; i < flashDuration; ++i) {
    // Randomly alternate the color of the start LED between white and its hue.
    leds[startLed] = (random8(10) >= 5) ? CRGB::White : CRGB(CHSV(startHue, 255, 255));
    FastLED.show();
    FastLED.delay(random8(20, 60)); // Random delay for the flickering effect.
  }

  // Create a rapidly increasing flash rate effect on the start LED.
  unsigned short rapidFlashDuration = random8(10, 20); // Duration for the rapid flashing.
  for (unsigned short i = 0; i < rapidFlashDuration; ++i) {
    leds[startLed] = (i % 2 == 0) ? CRGB::White : CRGB(CHSV(startHue, 255, 255));
    FastLED.show();
    FastLED.delay(i);
  }

  // Restore the start LED's color.
  leds[startLed] = CHSV(startHue, 255, 255);
  FastLED.show();
  FastLED.delay(random8(20, 60)); // Random delay before moving on.

  // Initialize an array to track the brightness level of each LED.
  unsigned char ledBrightness[numLEDs] = {0};

  // Gradually increase the brightness of each LED to create a rainbow effect.
  for (unsigned short offset = 1; offset < numLEDs; offset++) {
    for (unsigned short i = 0; i < numLEDs; i++) {
      if (i == startLed) continue; // Skip the start LED.

      // Calculate the distance from the current LED to the start LED.
      unsigned int distance = abs(static_cast<int>(i) - static_cast<int>(startLed));

      // If the current LED is within the offset, adjust its brightness and color.
      if (distance <= offset) {
        int hueOffset = distance * 5; // Determine the color spread in the rainbow.
        unsigned char hue = (startHue + hueOffset) % 256;
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
  for (int step = 0; step < 255; step += 5) {
    bool updateNeeded = false;
    for (unsigned int i = 0; i < numLEDs; i++) {
      if (i == startLed) continue; // Skip the start LED.

      // Increase brightness only if not already at maximum.
      if (ledBrightness[i] < 255) {
        ledBrightness[i] = min(ledBrightness[i] + 5, 255);
        unsigned int distance = abs(static_cast<int>(i) - static_cast<int>(startLed));
        unsigned int hueOffset = distance * 5; // Determine the color spread in the rainbow.
        unsigned char hue = (startHue + hueOffset) % 256;
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
