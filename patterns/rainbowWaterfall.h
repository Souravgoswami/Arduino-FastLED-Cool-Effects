void rainbowWaterfall(int waveLength, int speed, int numLEDs) {
  static int position = 0;
  // Create a moving "wave" effect by looping through each LED
  for (int i = 0; i < numLEDs; i++) {
    // Generate a rainbow color based on the LED's position and the current time
    uint8_t hue = (i * 256 / numLEDs) + position;
    CRGB color = CHSV(hue, 255, 255);

    // Calculate the brightness of the LED based on a sine wave
    int brightness = (sin8(((i * 256 / waveLength) + position) % 256) - 128) * 2;
    brightness = max(0, brightness); // Ensure brightness is not negative

    leds[i] = color;
    leds[i].fadeToBlackBy(255 - brightness);
  }

  // Move the "wave" position for the next frame
  position += speed;
  if (position >= 256) position -= 256; // Wrap position
}
