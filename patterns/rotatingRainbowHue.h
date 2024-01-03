void rotatingRainbowHue(unsigned short numLED) {
  static unsigned short rotatingRainbowStartHue = 0;

  fill_rainbow(leds, numLED, rotatingRainbowStartHue, 7);
  rotatingRainbowStartHue = (rotatingRainbowStartHue + 1) % 256;
}
