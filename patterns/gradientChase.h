struct GradientChaseData {
  uint8_t hue = 0;
  uint32_t nextHueUpdateTime = 0;
  const uint8_t hueUpdateIntervalMillis = 1;
} gradientChaseData;

void gradientChaseClash(CRGB leds[], uint8_t numLEDs) {
  uint32_t currentTime = millis();

  for (uint8_t i = 0; i < numLEDs; i++) {
    // Calculate the position for the wave to start from both ends and clash in the middle
    float wavePos = fabs(i - (numLEDs / 2.0f));

    // Calculate wave-based brightness modulation with new wavePosition
    float wave = sin(wavePos * 0.15f + currentTime / 250.0f);
    uint8_t brightness = 127.5f + 127.5f * wave;

    // Make hue cycle 3x across strip, and slowly rotate with time
    uint8_t hue = gradientChaseData.hue
                + ((uint32_t)i * 3 * 256 / numLEDs)
                + (currentTime / 250);

    // Apply wave function to brightness and create gradient
    leds[i] = CHSV(hue, 255, brightness);
  }

  if (currentTime > gradientChaseData.nextHueUpdateTime) {
    gradientChaseData.hue++;
    gradientChaseData.nextHueUpdateTime = currentTime + gradientChaseData.hueUpdateIntervalMillis;
  }

  FastLED.show();
}

void gradientChase(CRGB leds[], uint8_t numLEDs, bool backwards) {
  uint32_t currentTime = millis();

  for (uint8_t i = 0; i < numLEDs; i++) {
    float wave = sin(i * 0.15f + currentTime / 250.0f);
    uint8_t brightness = 127.5f + 127.5f * wave;

    // Make hue cycle 3x across strip, and slowly rotate with time
    uint8_t hue = gradientChaseData.hue + ((uint32_t)i * 3 * 256 / numLEDs) + (currentTime / 250);

    leds[backwards ? i : numLEDs - 1 - i] = CHSV(hue, 255, brightness);
  }

  if (currentTime > gradientChaseData.nextHueUpdateTime) {
    gradientChaseData.hue++;
    gradientChaseData.nextHueUpdateTime = currentTime + gradientChaseData.hueUpdateIntervalMillis;
  }

  FastLED.show();
}
