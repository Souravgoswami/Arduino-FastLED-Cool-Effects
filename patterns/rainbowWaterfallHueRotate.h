// Function to create a moving rainbow effect with varying brightness like a waterfall
void rainbowWaterfallHueRotate(int waveLength, int speed, int numLEDs) {
  static int position = 0; // Static variable to hold the current position of the wave

  // Loop through each LED to set its color and brightness
  for (int i = 0; i < numLEDs; i++) {
    // Calculate the hue for each LED, creating a rainbow that moves along the strip
    uint8_t hue = (i * 256 / numLEDs) + position;
    CRGB color = CHSV(hue, 255, 255); // Create color with full saturation and maximum brightness

    // Use a sine function to calculate brightness modulation for a wave effect
    // The sine wave is based on the LED's position and the moving position variable
    int brightness = (sin8(((i * 256 / waveLength) + position) % 256) - 128) * 2;
    brightness = max(0, brightness); // Ensure the calculated brightness is not negative

    leds[i] = color; // Set the LED's color
    leds[i].fadeToBlackBy(255 - brightness); // Modulate the LED's brightness using fadeToBlackBy
  }

  // Increment the position to move the wave and the rainbow pattern along the strip
  position += speed;
  // Wrap the position around 256 to create a continuous loop effect
  if (position >= 256) position -= 256;

  FastLED.show(); // Display the updated LED colors and brightness
}
