void moveRainbowLight(int numLEDs, CRGB* leds) {
  static int position = 0; // Current position of the head of the light
  static int direction = 1; // Direction of the light movement: 1 for forward, -1 for backward
  static int hue = 0; // Starting hue value for the rainbow effect
  static int moveDelayFactor = 1250 / numLEDs;

  // Fade all LEDs by a small amount to create the trailing effect
  for (int i = 0; i < numLEDs; i++) {
    leds[i].fadeToBlackBy(30); // Adjust fade value (0-255) to control trail length
  }

  // Calculate the hue for the current position to create a rainbow effect
  // You can adjust the hue delta to control the spread of the rainbow colors
  int currentHue = hue + (position * 256 / numLEDs);
  leds[position] = CHSV(currentHue, 255, 255); // Set the head of the light to a bright color

  // Move the light position
  position += direction;
  // Update hue for a slowly changing rainbow effect
  hue += 7;

  // Change direction at the ends
  if (position == numLEDs - 1 || position == 0) {
    direction *= -1;
  }

  FastLED.show();
  FastLED.delay(moveDelayFactor);
}
