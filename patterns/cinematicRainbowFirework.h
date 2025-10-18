struct CinematicRainbowFireworkData {
  // Define minimum and maximum delay factors for the rainbow firework animation effects.
  static constexpr uint16_t delayFactorMin = 750; // ms per LED
  static constexpr uint16_t delayFactorMax = 1250; // ms per LED

  static constexpr uint16_t gradientClashDuration = 9000;  // ms

  static constexpr uint16_t hueRotateDuration = 8000;  // ms
  static constexpr uint16_t glitterBombWithHuePhaseDuration = 8000; // ms

  static constexpr uint16_t juggleDuration = 5000; // ms

  static constexpr uint8_t rotatingHueIncrement = 3;
  static constexpr uint8_t glitterBombWithHueGlitterChance = 60;  // 0–255 chance per frame for a sparkle

  static constexpr uint16_t randomSparkleDuration = 8500; // total duration in ms

  static constexpr uint16_t randomSparkleDelay = 30;      // time between random LEDs lighting up
  static constexpr uint8_t randomSparkleDensity = 6;      // number of LEDs to light each frame
};

void cinematicRainbowFirework(uint16_t numLEDs) {
  // Main animation logic.
  // Randomly select a starting LED for the animation.
  uint8_t startLed = random(numLEDs);

  // Gradually fade all LEDs to black.
  for (uint16_t i = 0; i < 256; i++) {
    if (ledData.modeButtonPressed) return;

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
    if (ledData.modeButtonPressed) return;

    // Randomly alternate the color of the start LED between white and its hue.
    leds[startLed] = (random8(10) >= 5) ? CRGB::White : CRGB(CHSV(startHue, 255, 255));
    FastLED.show();
    FastLED.delay(random8(20, 60)); // Random delay for the flickering effect.
  }

  // Create a rapidly increasing flash rate effect on the start LED.
  uint16_t rapidFlashDuration = random8(10, 20); // Duration for the rapid flashing.
  for (uint16_t i = 0; i < rapidFlashDuration; ++i) {
    if (ledData.modeButtonPressed) return;

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
  for (uint16_t offset = 1; offset < numLEDs; offset++) {
    if (ledData.modeButtonPressed) return;

    for (uint16_t i = 0; i < numLEDs; i++) {
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

    uint16_t delayPerLEDMin = CinematicRainbowFireworkData::delayFactorMin / numLEDs;
    uint16_t delayPerLEDMax = CinematicRainbowFireworkData::delayFactorMax / numLEDs;

    randomDelayDuration = random8(delayPerLEDMin, delayPerLEDMax);
    FastLED.delay(randomDelayDuration);
  }

  // Final brightness increase step for all LEDs.
  for (int16_t step = 0; step < 255; step += 5) {
    bool updateNeeded = false;
    for (uint16_t i = 0; i < numLEDs; i++) {
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

  // --- Hold the completed rainbow before glitter ---
  FastLED.show();
  FastLED.delay(500);

  // --- Store current rainbow colors BEFORE glitter ---
  CRGB baseColors[numLEDs];
  for (uint16_t i = 0; i < numLEDs; i++) {
    baseColors[i] = leds[i];
  }

  // --- Hue rotation phase: slowly rotate existing rainbow colors ---
  uint32_t hueRotateStart = millis();

  while (millis() - hueRotateStart < CinematicRainbowFireworkData::hueRotateDuration) {
    if (ledData.modeButtonPressed) return;

    for (uint16_t i = 0; i < numLEDs; i++) {
      CHSV hsv = rgb2hsv_approximate(leds[i]);
      hsv.hue += CinematicRainbowFireworkData::rotatingHueIncrement;
      leds[i] = hsv;
    }
    FastLED.show();
    FastLED.delay(12);
  }

  // --- Glitter with continuous hue rotation (in parallel) ---
  uint32_t phaseStart = millis();

  // Store base colors so we can restore the hue structure each frame
  for (uint16_t i = 0; i < numLEDs; i++) {
    baseColors[i] = leds[i];
  }

  while ((uint32_t)(millis() - phaseStart) < CinematicRainbowFireworkData::glitterBombWithHuePhaseDuration) {
    // --- Step 1: Rotate hues across all LEDs ---
    for (uint16_t i = 0; i < numLEDs; i++) {
      CHSV hsv = rgb2hsv_approximate(baseColors[i]);
      hsv.hue += ((millis() - phaseStart) / 10) + CinematicRainbowFireworkData::rotatingHueIncrement; // smoothly advancing hue
      leds[i] = hsv;
    }

    // --- Step 2: Add random white glitter flashes ---
    if (random8() < CinematicRainbowFireworkData::glitterBombWithHueGlitterChance) {
      uint16_t glitterIndex = random16(numLEDs);
      leds[glitterIndex] += CRGB::White;
    }

    FastLED.show();
    FastLED.delay(10); // frame delay for smoothness
  }

  // --- Juggle effect over the static rainbow ---
  uint32_t juggleStart = millis();
  while (millis() - juggleStart < CinematicRainbowFireworkData::juggleDuration) {
    if (ledData.modeButtonPressed) return;

    fadeToBlackBy(leds, numLEDs, 20);

    // Add moving colored dots over existing rainbow hues
    uint8_t dothue = 0;
    for (uint8_t i = 0; i < 8; ++i) {
      uint16_t pos = beatsin16(i + 7, 0, numLEDs - 1);
      leds[pos] += CHSV(dothue, 200, 255);
      dothue += 32;
    }

    FastLED.show();
    FastLED.delay(20);
  }

  // --- Gradient Chase Clash ---
  uint32_t clashStart = millis();
  uint8_t baseHue = random8();

  while ((uint32_t)(millis() - clashStart) < CinematicRainbowFireworkData::gradientClashDuration) {
    if (ledData.modeButtonPressed) return;

    uint32_t currentTime = millis();

    for (uint16_t i = 0; i < numLEDs; i++) {
      // Position mirrored around the center (waves clashing in middle)
      float wavePos = abs(i - (numLEDs / 2.0f));

      // Smooth sine wave controlling brightness — synchronized motion
      float wave = sin(wavePos * 0.18f + currentTime / 250.0f);
      uint8_t brightness = 127.5f + 127.5f * wave;

      // Gradient hue pattern that evolves over time
      uint8_t hue = baseHue + ((i * 256) / 15) + (currentTime / 15);

      leds[i] = CHSV(hue, 255, brightness);
    }

    FastLED.show();
    FastLED.delay(10);
  }

  // --- Random base-color sparkle phase ---
  uint32_t sparkleStart = millis();

  // Generate a new base rainbow palette to pull colors from
  for (uint16_t i = 0; i < numLEDs; i++) {
    uint8_t hue = (i * 256 / numLEDs);
    baseColors[i] = CHSV(hue, 255, 255);
  }

  FastLED.clear();

  while ((uint32_t)(millis() - sparkleStart) < CinematicRainbowFireworkData::randomSparkleDuration) {
    if (ledData.modeButtonPressed) return;

    // Light up a few random LEDs in their rainbow color
    for (uint8_t n = 0; n < CinematicRainbowFireworkData::randomSparkleDensity; n++) {
      uint16_t idx = random16(numLEDs);
      leds[idx] = baseColors[idx];
    }

    FastLED.show();
    FastLED.delay(CinematicRainbowFireworkData::randomSparkleDelay);

    // Optional: slowly fade everything for a twinkle effect
    fadeToBlackBy(leds, numLEDs, 40);
  }
}
