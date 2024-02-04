void staticFullChainRainbow(CRGB* leds, short numLEDs) {
  float hueIncrement = 255.0 / numLEDs; // Spread the rainbow across all LEDs
  for (int i = 0; i < numLEDs; i++) {
    leds[i] = CHSV(hueIncrement * i, 255, 255); // Full brightness for initial setup
  }
}
