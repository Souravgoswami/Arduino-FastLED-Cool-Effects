struct RainbowData {
  uint8_t startHue = 0;
  bool fadeOutFlags[MAX_LED_COUNT] = {false};
} rainbowData;

void rainbow2(uint8_t numLED, uint16_t turnOffNum) {
  // Update the rainbow for LEDs not marked for fading
  for (uint8_t i = 0; i < numLED; ++i) {
    if (!rainbowData.fadeOutFlags[i]) {
      leds[i] = ColorFromPalette(RainbowColors_p, rainbowData.startHue + i * 7, 255, LINEARBLEND);
    }
  }

  rainbowData.startHue++;

  // Process fading out for marked LEDs
  for (uint8_t i = 0; i < numLED; ++i) {
    if (rainbowData.fadeOutFlags[i]) {
      leds[i].fadeToBlackBy(2);
      if (leds[i].getAverageLight() == 0) {
        rainbowData.fadeOutFlags[i] = false;
      }
    }
  }

  // Randomly select LEDs to fade out
  for (uint16_t i = 0; i < turnOffNum; ++i) {
    if (random8() < 20) {
      uint8_t pos = random8(numLED);
      rainbowData.fadeOutFlags[pos] = true;
    }
  }
}
