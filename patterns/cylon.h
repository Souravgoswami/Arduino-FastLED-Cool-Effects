struct Cylon2Data {
  uint8_t brightness[MAX_LED_COUNT] = {0};
  uint8_t baseHue = 0;
} cylon2Data;

void cylon(uint8_t numLEDs) {
  static uint8_t hue = 0;

  for(uint8_t i = 0; i < numLEDs; ++i) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();

    for(uint16_t j = 0; j < numLEDs; ++j) {
      leds[j].nscale8(250);
    }

    FastLED.delay(10);
  }

  for(uint16_t i = numLEDs - 1; i > 0; --i) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();

    for(uint16_t j = 0; j < numLEDs; ++j) {
      leds[j].nscale8(250);
    }

    FastLED.delay(10);
  }
}

void cylon2(uint8_t numLEDs) {
  for (uint8_t i = 0; i < numLEDs; ++i) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    // Light up the current LED with maximum brightness
    cylon2Data.brightness[i] = 255;

    // Gradually fade all LEDs and update their brightness
    for (uint16_t j = 0; j < numLEDs; ++j) {
      uint8_t hue = cylon2Data.baseHue + (j * 256 / numLEDs);
      leds[j] = CHSV(hue, 255, cylon2Data.brightness[j]);
      cylon2Data.brightness[j] = qsub8(cylon2Data.brightness[j], 3);
    }

    FastLED.show();
    FastLED.delay(10);

    cylon2Data.baseHue++;
  }

  for (uint8_t i = numLEDs - 1; i > 0; --i) {
    if (ledData.modeButtonPressed || ledData.ledChainToggleButtonPressed) return;

    // Light up the current LED with maximum brightness
    cylon2Data.brightness[i] = 255;

    // Gradually fade all LEDs and update their brightness
    for (uint8_t j = 0; j < numLEDs; ++j) {
      uint8_t hue = cylon2Data.baseHue + (j * 256 / numLEDs);
      leds[j] = CHSV(hue, 255, cylon2Data.brightness[j]);
      cylon2Data.brightness[j] = qsub8(cylon2Data.brightness[j], 3);
    }

    FastLED.show();
    FastLED.delay(10);

    cylon2Data.baseHue++;
  }
}
