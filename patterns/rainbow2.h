unsigned short startHue = 0;
bool fadeOutFlags[MAX_LED_COUNT] = {false};

void rainbow2(unsigned short numLED, unsigned int turnOffNum) {
  // Update the rainbow for LEDs not marked for fading
  for (unsigned int i = 0; i < numLED; ++i) {
    if (!fadeOutFlags[i]) {
      leds[i] = ColorFromPalette(RainbowColors_p, startHue + i * 7, 255, LINEARBLEND);
    }
  }

  startHue = (startHue + 1) % 256;

  // Process fading out for marked LEDs
  for (unsigned int i = 0; i < numLED; ++i) {
    if (fadeOutFlags[i]) {
      leds[i].fadeToBlackBy(2);
      if (leds[i].getAverageLight() == 0) {
        fadeOutFlags[i] = false;
      }
    }
  }

  // Randomly select LEDs to fade out
  for (unsigned int i = 0; i < turnOffNum; ++i) {
    if (random8() < 20) {
      int pos = random16(numLED);
      fadeOutFlags[pos] = true;
    }
  }
}
