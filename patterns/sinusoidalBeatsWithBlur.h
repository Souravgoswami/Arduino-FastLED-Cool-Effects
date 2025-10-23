void sinusoidalBeatsWithBlur(uint8_t numLED) {
  float scaleFactor = (numLED - 1) / 255.0;

  // Apply scaling factor to beatsin8 output
  uint16_t sinBeat1 = round(beatsin8(30, 0, 255, 0, 0) * scaleFactor);
  uint16_t sinBeat2 = round(beatsin8(30, 0, 255, 0, 42) * scaleFactor);
  uint16_t sinBeat3 = round(beatsin8(30, 0, 255, 0, 84) * scaleFactor);
  uint16_t sinBeat4 = round(beatsin8(30, 0, 255, 0, 126) * scaleFactor);
  uint16_t sinBeat5 = round(beatsin8(30, 0, 255, 0, 168) * scaleFactor);
  uint16_t sinBeat6 = round(beatsin8(30, 0, 255, 0, 210) * scaleFactor);

  leds[sinBeat1] = CRGB(0x00, 0xff, 0x00);
  leds[sinBeat2] = CRGB(0xff, 0xff, 0x00);
  leds[sinBeat3] = CRGB(0x3c, 0xe3, 0xb4);
  leds[sinBeat4] = CRGB(0x44, 0x44, 0xff);
  leds[sinBeat5] = CRGB(0xff, 0x50, 0xa6);
  leds[sinBeat6] = CRGB(0x55, 0x55, 0xff);

  EVERY_N_MILLISECONDS(1) {
    for(uint8_t i = 0; i < 4; ++i) {
      blur1d(leds, numLED, 100);
    }
  }

  FastLED.show();
}
