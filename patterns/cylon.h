struct Cylon2Data {
  uint8_t brightness[MAX_LED_COUNT] = {0};
  uint8_t baseHue = 0;
} cylon2Data;

void cylon(uint16_t numLEDs) {
  static uint8_t hue = 0;

  for(uint16_t i = 0; i < numLEDs; ++i) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();

    for(uint16_t j = 0; j < numLEDs; ++j) {
      leds[j].nscale8(250);
    }

    FastLED.delay(10);
  }

  for(uint16_t i = numLEDs - 1; i > 0; --i) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();

    for(uint16_t j = 0; j < numLEDs; ++j) {
      leds[j].nscale8(250);
    }

    FastLED.delay(10);
  }
}

void cylon2(uint16_t numLEDs) {
  for (uint16_t i = 0; i < numLEDs; ++i) {
    // Light up the current LED with maximum brightness
    cylon2Data.brightness[i] = 255;

    // Gradually fade all LEDs and update their brightness
    for (uint16_t j = 0; j < numLEDs; ++j) {
      uint8_t hue = cylon2Data.baseHue + (j * 256 / numLEDs);
      leds[j] = CHSV(hue, 255, cylon2Data.brightness[j]);
      cylon2Data.brightness[j] = (cylon2Data.brightness[j] * 245) / 255;
    }

    FastLED.show();
    FastLED.delay(10);

    cylon2Data.baseHue++;
  }

  for (uint16_t i = numLEDs - 1; i > 0; --i) {
    // Light up the current LED with maximum brightness
    cylon2Data.brightness[i] = 255;

    // Gradually fade all LEDs and update their brightness
    for (uint16_t j = 0; j < numLEDs; ++j) {
      uint8_t hue = cylon2Data.baseHue + (j * 256 / numLEDs);
      leds[j] = CHSV(hue, 255, cylon2Data.brightness[j]);
      cylon2Data.brightness[j] = (cylon2Data.brightness[j] * 245) / 255;
    }

    FastLED.show();
    FastLED.delay(10);

    cylon2Data.baseHue++;
  }
}
