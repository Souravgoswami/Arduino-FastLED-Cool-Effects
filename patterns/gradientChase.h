struct GradientChaseData {
  uint16_t hue = 0;

};

GradientChaseData gradientChaseData;

void gradientChaseClash(CRGB leds[], int numLEDs) {
  for (uint8_t i = 0; i < numLEDs; i++) {
    // Calculate the position for the wave to start from both ends and clash in the middle
    float wavePosition = abs(i - (numLEDs / 2.0));

    // Calculate wave-based brightness modulation with new wavePosition
    uint8_t brightness = 127.5 + 127.5 * sin(wavePosition * 0.15 + millis() / 250.0);

    // Calculate the hue based on position to create a gradient
    uint8_t hue = (gradientChaseData.hue + (i * 256 / 15)) % 256;

    // Apply wave function to brightness and create gradient
    leds[i] = CHSV(hue, 255, brightness);
  }

  gradientChaseData.hue = (gradientChaseData.hue + 1) % 256;
  FastLED.show();
}

void gradientChase(CRGB leds[], int numLEDs, bool backwards) {
  for (uint8_t i = 0; i < numLEDs; i++) {
    // Calculate wave-based brightness modulation
    uint8_t brightness = 127.5 + 127.5 * sin(i * 0.15 + millis() / 250.0);

    // Calculate the hue offset based on position to create a gradient
    uint8_t hue = (gradientChaseData.hue + (i * 256 / 15)) % 256;

    // Apply wave function to brightness and create gradient
    leds[backwards ? i : numLEDs - 1 - i] = CHSV(hue, 255, brightness);
  }

  gradientChaseData.hue = (gradientChaseData.hue + 1) % 256;
  FastLED.show();
}
