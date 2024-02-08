void rainbowFirework(unsigned short numLEDs) {
  unsigned char startLed = random(numLEDs);
  unsigned int delayFactorMin = 750; // 7.5ms for 1 LED, 1250 for 100
  unsigned int delayFactorMax = 1250; // 12.5ms for 1 LED, 1250 for 100

  unsigned int delayPerLEDMin = delayFactorMin / numLEDs;
  unsigned int delayPerLEDMax = delayFactorMax / numLEDs;

  for (unsigned short i = 0; i < 256; i++) {
    fadeToBlackBy(leds, numLEDs, 1);
    FastLED.show();
    FastLED.delay(5);
  }

  FastLED.clear();
  FastLED.show();
  FastLED.delay(5);

  unsigned char startHue = 255; // random(256);
  leds[startLed] = CHSV(startHue, 255, 255);
  FastLED.show();

  unsigned short randomDelayDuration = random16(250, 500);
  FastLED.delay(randomDelayDuration);

  // Fizzling effect on the startLed
  unsigned short flashDuration = random8(10, 25);  // Random duration for fizzling effect
  for(unsigned short i = 0; i < flashDuration; ++i) {
    if (random8(10) >= 5) {
      leds[startLed] = CRGB::White;
    } else {
      leds[startLed] = CHSV(startHue, 255, 255);
    }

    FastLED.show();
    FastLED.delay(random8(20, 60));  // Random delay for flickering effect
  }

  // Rapidly increasing flash rate
  unsigned short rapidFlashDuration = random8(10, 20); // Duration for rapid flashing
  for (unsigned short i = 0; i < rapidFlashDuration; ++i) {
    leds[startLed] = (i % 2 == 0) ? CRGB::White : CRGB(CHSV(startHue, 255, 255));

    FastLED.show();
    FastLED.delay(i);
  }

  leds[startLed] = CHSV(startHue, 255, 255);
  FastLED.show();
  FastLED.delay(random8(20, 60));  // Random delay for restoring original colour

  // Array to keep track of the brightness of each LED
  unsigned char ledBrightness[numLEDs] = {0};

  // Gradually fill each LED with the rainbow, one at a time
  for (unsigned short offset = 1; offset < numLEDs; offset++) {
    for (unsigned short i = 0; i < numLEDs; i++) {
      if (i == startLed) {
        continue; // Skip the startLED
      }

      unsigned int distance = abs(static_cast<int>(i) - static_cast<int>(startLed));

      if (distance <= offset) {
        int hueOffset = distance * 5; // Determines the color spread in the rainbow
        unsigned char hue = (startHue + hueOffset) % 256;

        ledBrightness[i] = min(ledBrightness[i] + 20, 255);
        leds[i] = CHSV(hue, 255, ledBrightness[i]);
      }
    }

    randomDelayDuration = random8(delayPerLEDMin, delayPerLEDMax);

    FastLED.show();
    FastLED.delay(randomDelayDuration);
  }

  for (int step = 0; step < 255; step += 5) {
      bool updateNeeded = false;
      for (unsigned int i = 0; i < numLEDs; i++) {
        if (i == startLed) continue; // Skip the startLED

        // Only increase brightness if it's not already at maximum
        if (ledBrightness[i] < 255) {
            ledBrightness[i] = min(ledBrightness[i] + 5, 255);

            unsigned int distance = abs(static_cast<int>(i) - static_cast<int>(startLed));
            unsigned int hueOffset = distance * 5; // Determines the color spread in the rainbow
            unsigned char hue = (startHue + hueOffset) % 256;

            leds[i] = CHSV(hue, 255, ledBrightness[i]);
            updateNeeded = true;
        }
      }

      if (updateNeeded) {
        FastLED.show();
      }
  }

  FastLED.delay(1000);
}
