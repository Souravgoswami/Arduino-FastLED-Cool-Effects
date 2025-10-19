void moveRainbowLight(uint16_t numLEDs, CRGB* leds) {
  static uint16_t position = 0; // Current position of the head of the light
  static int8_t direction = 1; // Direction of the light movement: 1 for forward, -1 for backward
  static uint8_t hue = 0; // Starting hue value for the rainbow effect
  static uint16_t moveDelayFactor = 1250 / numLEDs;

  // Fade all LEDs by a small amount to create the trailing effect
  for (uint8_t i = 0; i < numLEDs; i++) {
    leds[i].fadeToBlackBy(30); // Adjust fade value (0-255) to control trail length
  }

  // Calculate the hue for the current position to create a rainbow effect
  float hueIncrement = 256.0 * position / numLEDs;
  uint16_t currentHue = hue + static_cast<uint16_t>(hueIncrement) % 256;

  leds[position] = CHSV(currentHue, 255, 255); // Set the head of the light to a bright color

  // Move the light position
  position += direction;
  // Update hue for a slowly changing rainbow effect
  hue++;

  // Change direction at the ends
  if (position == numLEDs - 1 || position == 0) {
    direction *= -1;
  }

  FastLED.show();
  FastLED.delay(moveDelayFactor);
}
