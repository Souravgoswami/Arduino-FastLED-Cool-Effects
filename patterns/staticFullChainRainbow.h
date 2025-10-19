void staticFullChainRainbow(CRGB* leds, uint16_t numLEDs) {
  float hueIncrement = 255.0 / numLEDs; // Spread the rainbow across all LEDs
  for (uint8_t i = 0; i < numLEDs; i++) {
    leds[i] = CHSV(hueIncrement * i, 255, 255); // Full brightness for initial setup
  }
}
