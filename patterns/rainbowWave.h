void rainbowWave(int wait, int numLEDs) {
  static uint8_t hue = 0;
  // First, fill the strip with a moving rainbow.
  for(int i = 0; i < numLEDs; i++) {
    leds[i] = CHSV((i * 10) + hue, 255, 255);
  }
  // Show the LEDs and increase the starting hue.
  FastLED.show();
  hue += 2;
  FastLED.delay(wait);
}
