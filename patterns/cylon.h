void cylon(uint16_t numLEDs) {
  static uint8_t hue = 0;

  for(uint16_t i = 0; i < numLEDs; ++i) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    for(uint16_t i = 0; i < numLEDs; ++i) leds[i].nscale8(250);
    FastLED.delay(10);
  }

  for(uint16_t i = numLEDs - 1; i; --i) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    for(uint16_t i = 0; i < numLEDs; ++i) leds[i].nscale8(250);
    FastLED.delay(10);
  }
}
