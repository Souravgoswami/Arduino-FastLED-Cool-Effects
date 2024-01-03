struct MetorRainData {
  float meteorSizeFactor = 0.2; // 20 /  100
  float meteorTrailDecayFactor = 0.5; // 64 / 100
  float meteorTrailLengthFactor = 0.25; // 25 / 100
  unsigned short meteorDelayFactor = 1000; // 10 * 100

  bool meteorRandomDecay = true;

} __attribute__((packed));

MetorRainData metorRainData;

void meteorRain(uint32_t colour, unsigned short numLED) {
  unsigned short meteorSize = max(1, (unsigned short)(numLED * metorRainData.meteorSizeFactor));
  unsigned char meteorTrailDecay = max(1, (unsigned char)(255 * metorRainData.meteorTrailDecayFactor));
  unsigned short meteorTrailLength = max(1, (unsigned short)(numLED * metorRainData.meteorTrailLengthFactor));

  unsigned char r = colour >> 16;
  unsigned char g = (colour >> 8) & 0xff;
  unsigned char b = colour & 0xff;

  FastLED.clear();

  for (unsigned short i = 0; i < numLED + meteorTrailLength; ++i) {
    // Draw the meteor
    for (unsigned short j = 0; j < meteorSize; ++j) {
      if (i - j < numLED) {
        leds[i - j] = CRGB(r, g, b);
      }
    }

    // Draw the tail
    for (unsigned short j = 0; j < numLED; ++j) {
      if (metorRainData.meteorRandomDecay && (random(10) > 3)) {
        leds[j].fadeToBlackBy(meteorTrailDecay);
      }
    }

    FastLED.show();
    FastLED.delay(metorRainData.meteorDelayFactor / numLED);
  }
}
