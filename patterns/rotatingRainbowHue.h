void rotatingRainbowHue(uint8_t numLED) {
  static uint8_t rotatingRainbowStartHue = 0;

  fill_rainbow(leds, numLED, rotatingRainbowStartHue, 7);
  rotatingRainbowStartHue++;
}
