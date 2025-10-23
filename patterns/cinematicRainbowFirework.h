struct CinematicRainbowFireworkData {
  // Define minimum and maximum delay factors for the rainbow firework animation effects.
  static constexpr uint16_t delayFactorMin = 750; // ms per LED
  static constexpr uint16_t delayFactorMax = 1250; // ms per LED

  static constexpr uint16_t gradientClashDuration = 11000;  // ms
  static constexpr uint16_t gradientClashHueUpdateIntervalMillis = 1;

  static constexpr uint16_t hueRotateDuration = 8000;  // ms
  static constexpr uint16_t glitterBombWithHuePhaseDuration = 8000; // ms

  static constexpr uint16_t juggleDuration = 9000; // ms

  static constexpr uint8_t rotatingHueIncrement = 3;
  static constexpr uint8_t glitterBombWithHueGlitterChance = 60;  // 0–255 chance per frame for a sparkle

  static constexpr uint16_t randomSparkleDuration = 10000; // total duration in ms

  static constexpr uint16_t randomSparkleDelay = 30;      // time between random LEDs lighting up
  static constexpr uint8_t randomSparkleDensity = 6;      // number of LEDs to light each frame

  static constexpr float meteorSizeFactor = 0.2;  // n /  100
  static constexpr float meteorTrailDecayFactor = 0.75;  // n / 100
  static constexpr float meteorTrailLengthFactor = 0.25;  // n / 100
  static constexpr uint16_t meteorDelayFactor = 2500;  // n * 100

  static constexpr uint16_t rainbowRevealHueRotateDuration = 3000; // ms
};

void cinematicRainbowFirework(uint8_t numLEDs) {
  // Main animation logic.
  // Randomly select a starting LED for the animation.
  uint8_t startLed = random8(numLEDs);

  // Gradually fade all LEDs to black.
  for (uint8_t i = 0; i < 255; i++) {
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
  uint8_t startHue = random8();
  leds[startLed] = CHSV(startHue, 255, 255);
  FastLED.show();

  // Pause for a random duration between animations.
  uint16_t randomDelayDuration = random16(250, 500);
  FastLED.delay(randomDelayDuration);

  // Create a fizzling effect on the start LED.
  // Duration for the fizzling effect.
  uint8_t flashDuration = random8(10, 25);
  for(uint8_t i = 0; i < flashDuration; ++i) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    // Randomly alternate the color of the start LED between white and its hue.
    leds[startLed] = (random8(10) >= 5) ? CRGB::White : CRGB(CHSV(startHue, 255, 255));
    FastLED.show();
    FastLED.delay(random8(20, 60)); // Random delay for the flickering effect.
  }

  // Create a rapidly increasing flash rate effect on the start LED.
  uint8_t rapidFlashDuration = random8(10, 20); // Duration for the rapid flashing.
  for (uint8_t i = 0; i < rapidFlashDuration; ++i) {
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
  uint8_t numLEDSizedAry[numLEDs];

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
        numLEDSizedAry[i] = min(numLEDSizedAry[i] + 20, 255);
        leds[i] = CHSV(hue, 255, numLEDSizedAry[i]);
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
    for (uint8_t i = 0; i < numLEDs; i++) {
      if (i == startLed) continue; // Skip the start LED.

      // Increase brightness only if not already at maximum.
      if (numLEDSizedAry[i] < 255) {
        numLEDSizedAry[i] = min(numLEDSizedAry[i] + 5, 255);
        uint16_t distance = abs(static_cast<int16_t>(i) - static_cast<int16_t>(startLed));
        uint16_t hueOffset = distance * 5; // Determine the color spread in the rainbow.
        uint8_t hue = startHue + hueOffset;
        leds[i] = CHSV(hue, 255, numLEDSizedAry[i]);
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
  for (uint8_t i = 0; i < numLEDs; i++) {
    baseColors[i] = leds[i];
  }

  // --- Hue rotation phase: slowly rotate existing rainbow colors ---
  uint32_t hueRotateStart = millis();

  while (millis() - hueRotateStart < CinematicRainbowFireworkData::hueRotateDuration) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    for (uint8_t i = 0; i < numLEDs; i++) {
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
  for (uint8_t i = 0; i < numLEDs; i++) {
    baseColors[i] = leds[i];
  }

  while ((uint32_t)(millis() - phaseStart) < CinematicRainbowFireworkData::glitterBombWithHuePhaseDuration) {
    // --- Step 1: Rotate hues across all LEDs ---
    for (uint8_t i = 0; i < numLEDs; i++) {
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
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    fadeToBlackBy(leds, numLEDs, 20);

    // Add moving colored dots over existing rainbow hues
    uint8_t dothue = startHue;
    for (uint8_t i = 0; i < 8; ++i) {
      uint16_t pos = beatsin16(i + 7, 0, numLEDs - 1);
      leds[pos] += CHSV(dothue, 200, 255);
      dothue += 32;
    }

    FastLED.show();
    FastLED.delay(20);
  }

  // --- Random base-color sparkle phase ---
  uint32_t sparkleStart = millis();

  while ((uint32_t)(millis() - sparkleStart) < CinematicRainbowFireworkData::randomSparkleDuration) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    // Light up a few random LEDs in their rainbow color
    for (uint8_t n = 0; n < CinematicRainbowFireworkData::randomSparkleDensity; n++) {
      uint16_t idx = random8(numLEDs);
      leds[idx] = baseColors[idx];
    }

    FastLED.show();
    FastLED.delay(CinematicRainbowFireworkData::randomSparkleDelay);

    // Optional: slowly fade everything for a twinkle effect
    fadeToBlackBy(leds, numLEDs, 40);

    // Generate a new base rainbow palette to pull colors from
    for (uint8_t i = 0; i < numLEDs; i++) {
      CHSV hsv = rgb2hsv_approximate(baseColors[i]);
      hsv.hue += CinematicRainbowFireworkData::rotatingHueIncrement;  // small hue shift
      baseColors[i] = hsv;
    }
  }

  // --- Gradient Chase Clash ---
  uint32_t clashStart = millis();
  uint8_t baseHue = rgb2hsv_approximate(baseColors[0]).hue;
  uint32_t lastHueUpdate = millis();

  while ((millis() - clashStart) < CinematicRainbowFireworkData::gradientClashDuration) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;
    uint32_t currentTime = millis();

    // Increment base hue every few ms for visible motion
    if (currentTime - lastHueUpdate > CinematicRainbowFireworkData::gradientClashHueUpdateIntervalMillis) { // about 60 updates/sec
      baseHue++;
      lastHueUpdate = currentTime;
    }

    for (uint8_t i = 0; i < numLEDs; i++) {
      float wavePos = fabs(i - (numLEDs / 2.0f));
      float wave = sin(wavePos * 0.15f + currentTime / 250.0f);
      uint8_t brightness = 127.5f + 127.5f * wave;

      uint8_t hue = baseHue + ((uint32_t)i * 3 * 256 / numLEDs) + (currentTime / 250);

      leds[i] = CHSV(hue, 255, brightness);
    }

    FastLED.show();
    FastLED.delay(10);
  }

  // --- Random base-color sparkle phase ---
  sparkleStart = millis();

  while ((uint32_t)(millis() - sparkleStart) < CinematicRainbowFireworkData::randomSparkleDuration) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    // Light up a few random LEDs in their rainbow color
    for (uint8_t n = 0; n < CinematicRainbowFireworkData::randomSparkleDensity; n++) {
      uint8_t idx = random8(numLEDs);
      leds[idx] = baseColors[idx];
    }

    FastLED.show();
    FastLED.delay(CinematicRainbowFireworkData::randomSparkleDelay);

    // Optional: slowly fade everything for a twinkle effect
    fadeToBlackBy(leds, numLEDs, 40);

    // Generate a new base rainbow palette to pull colors from
    for (uint8_t i = 0; i < numLEDs; i++) {
      CHSV hsv = rgb2hsv_approximate(baseColors[i]);
      hsv.hue += CinematicRainbowFireworkData::rotatingHueIncrement;  // small hue shift
      baseColors[i] = hsv;
    }
  }

  // --- Center-Out Meteor Rain Sequence ---
  uint16_t meteorSize = max(1, static_cast<int>(numLEDs * CinematicRainbowFireworkData::meteorSizeFactor));
  uint8_t meteorTrailDecay = max(1, static_cast<int>(255 * CinematicRainbowFireworkData::meteorTrailDecayFactor));
  uint16_t meteorTrailLength = max(1, static_cast<int>(numLEDs * CinematicRainbowFireworkData::meteorTrailLengthFactor));
  uint16_t meteorDelay = CinematicRainbowFireworkData::meteorDelayFactor / numLEDs;
  uint8_t center = numLEDs / 2;

  // Meteor travels outward from center toward both ends
  for (uint8_t offset = 0; offset < center + meteorTrailLength; ++offset) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    // Left side meteor
    int16_t leftPos = center - offset;
    for (uint16_t j = 0; j < meteorSize; ++j) {
      int16_t idx = leftPos - j;
      if (idx >= 0 && idx < numLEDs) {
        leds[idx] = baseColors[idx];  // rainbow preserved
      }
    }

    // Right side meteor
    int16_t rightPos = center + offset;
    for (uint16_t j = 0; j < meteorSize; ++j) {
      int16_t idx = rightPos + j;
      if (idx >= 0 && idx < numLEDs) {
        leds[idx] = baseColors[idx];
      }
    }

    // Apply trail decay
    for (uint16_t j = 0; j < numLEDs; ++j) {
      if (random8(10) > 3) {
        leds[j].fadeToBlackBy(meteorTrailDecay);
      }
    }

    FastLED.show();
    FastLED.delay(meteorDelay);
  }

  // --- Random Rainbow Reveal with per-LED rotating hue ---
  // Optimize: Reuse CRGB baseColors array (otherwise it becomes impossible to run on an Arduino)
  for (uint8_t i = 0; i < numLEDs; i++) {
    baseColors[i].r = i;      // store LED index
    baseColors[i].g = 0;      // revealed flag = false
    baseColors[i].b = 0;      // optional: spare
  }

  // Step 2. Fisher–Yates shuffle on .r field
  for (uint8_t i = numLEDs - 1; i > 0; i--) {
    uint8_t j = random8(i + 1);

    uint8_t tmp = baseColors[i].r;
    baseColors[i].r = baseColors[j].r;
    baseColors[j].r = tmp;
  }

  uint8_t globalHueOffset = random8();

  // Record initial hue for each LED from baseColors
  for (uint8_t i = 0; i < numLEDs; i++) {
    CHSV hsv = rgb2hsv_approximate(baseColors[i]);
    numLEDSizedAry[i] = hsv.hue;
  }

  // Reveal LEDs one by one, each starts rotating once revealed
  for (uint8_t revealIndex = 0; revealIndex < numLEDs; revealIndex++) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    uint8_t idx = baseColors[revealIndex].r;
    // Mark this LED as revealed
    baseColors[idx].g = 1;

    uint32_t startTime = millis();
    uint32_t lastFrame = 0;

    // Keep updating for a short period after each new LED reveal
    uint8_t subPhaseDuration = random8(25, 60);
    while (millis() - startTime < subPhaseDuration) {
      if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

      uint32_t t = millis();

      for (uint8_t i = 0; i < numLEDs; i++) {
        // only update revealed LEDs
        if (!baseColors[i].g) continue;

        // Compute hue offset based on elapsed time
        // per-LED drift
        uint8_t hueRotated = ((t / 10) + i * 2) % 255;
        leds[i] = CHSV(numLEDSizedAry[i] + globalHueOffset + hueRotated, 255, 255);
      }

      FastLED.show();
      FastLED.delay(15);
    }
  }

  // Keep rotating all LEDs for a bit after all are revealed
  uint32_t rotateStart = millis();

  while (millis() - rotateStart < CinematicRainbowFireworkData::rainbowRevealHueRotateDuration) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    uint32_t t = millis();

    for (uint8_t i = 0; i < numLEDs; i++) {
      uint8_t hueRot = ((t / 10) + i * 2) % 255;
      leds[i] = CHSV(numLEDSizedAry[i] + globalHueOffset + hueRot, 255, 255);
    }

    FastLED.show();
    FastLED.delay(15);
  }
}
