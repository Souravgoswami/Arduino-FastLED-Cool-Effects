struct MetorRainData {
  static constexpr float meteorSizeFactor = 0.2; // 20 /  100
  static constexpr float meteorTrailDecayFactor = 0.5; // 64 / 100
  static constexpr float meteorTrailLengthFactor = 0.25; // 25 / 100
  static constexpr uint16_t meteorDelayFactor = 1000; // 10 * 100
  static constexpr bool meteorRandomDecay = true;
};

void meteorRain(uint32_t colour, uint8_t numLED) {
  uint16_t meteorSize = max(static_cast<int16_t>(1), static_cast<int16_t>(numLED * MetorRainData::meteorSizeFactor));
  uint8_t meteorTrailDecay = max(static_cast<int16_t>(1), static_cast<int16_t>(255 * MetorRainData::meteorTrailDecayFactor));
  uint16_t meteorTrailLength = max(static_cast<int16_t>(1), static_cast<int16_t>(numLED * MetorRainData::meteorTrailLengthFactor));

  uint8_t r = colour >> 16;
  uint8_t g = (colour >> 8) & 0xff;
  uint8_t b = colour & 0xff;

  FastLED.clear();

  for (uint8_t i = 0; i < numLED + meteorTrailLength; ++i) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    // Draw the meteor
    for (uint16_t j = 0; j < meteorSize; ++j) {
      if (i - j < numLED) {
        leds[i - j] = CRGB(r, g, b);
      }
    }

    // Draw the tail
    for (uint8_t j = 0; j < numLED; ++j) {
      if (MetorRainData::meteorRandomDecay && (random(10) > 3)) {
        leds[j].fadeToBlackBy(meteorTrailDecay);
      }
    }

    FastLED.show();
    FastLED.delay(MetorRainData::meteorDelayFactor / numLED);
  }
}
