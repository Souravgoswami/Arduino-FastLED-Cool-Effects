struct MetorRainData {
  float meteorSizeFactor = 0.2; // 20 /  100
  float meteorTrailDecayFactor = 0.5; // 64 / 100
  float meteorTrailLengthFactor = 0.25; // 25 / 100
  uint16_t meteorDelayFactor = 1000; // 10 * 100
  bool meteorRandomDecay = true;
} metorRainData;

void meteorRain(uint32_t colour, uint16_t numLED) {
  uint16_t meteorSize = max(1, (int)(numLED * metorRainData.meteorSizeFactor));
  uint8_t meteorTrailDecay = max(1, (int)(255 * metorRainData.meteorTrailDecayFactor));
  uint16_t meteorTrailLength = max(1, (int)(numLED * metorRainData.meteorTrailLengthFactor));

  uint8_t r = colour >> 16;
  uint8_t g = (colour >> 8) & 0xff;
  uint8_t b = colour & 0xff;

  FastLED.clear();

  for (uint16_t i = 0; i < numLED + meteorTrailLength; ++i) {
    // Draw the meteor
    for (uint16_t j = 0; j < meteorSize; ++j) {
      if (i - j < numLED) {
        leds[i - j] = CRGB(r, g, b);
      }
    }

    // Draw the tail
    for (uint16_t j = 0; j < numLED; ++j) {
      if (metorRainData.meteorRandomDecay && (random(10) > 3)) {
        leds[j].fadeToBlackBy(meteorTrailDecay);
      }
    }

    FastLED.show();
    FastLED.delay(metorRainData.meteorDelayFactor / numLED);
  }
}
