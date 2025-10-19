void rainbowWaterfall(uint8_t waveLength, uint8_t speed, uint16_t numLEDs) {
  static int16_t position = 0;
  // Create a moving "wave" effect by looping through each LED
  for (uint8_t i = 0; i < numLEDs; i++) {
    // Generate a rainbow color based on the LED's position and the current time
    uint8_t hue = ((i * 256) / numLEDs);
    CRGB color = CHSV(hue, 255, 255);

    // Calculate the brightness of the LED based on a sine wave
    int16_t brightness = (sin8(((i * 256 / waveLength) + position) % 256) - 128) * 2;
    brightness = max(static_cast<int16_t>(0), brightness); // Ensure brightness is not negative

    leds[i] = color;
    leds[i].fadeToBlackBy(255 - brightness);
  }

  // Move the "wave" position for the next frame
  position += speed;
  if (position >= 256) position -= 256; // Wrap position
}

void reverseRainbowWaterfall(uint8_t waveLength, uint8_t speed, uint16_t numLEDs) {
  static int16_t position = 0;
  // Create a moving "wave" effect by looping through each LED in reverse order
  for (uint8_t i = 0; i < numLEDs; i++) {
    // Generate a rainbow color based on the LED's position and the current time, reversed
    uint8_t hue = ((numLEDs - i) * 256 / numLEDs);
    CRGB color = CHSV(hue, 255, 255);

    // Calculate the brightness of the LED based on a sine wave, reversed
    int16_t brightness = (sin8((((numLEDs - i) * 256 / waveLength) + position) % 256) - 128) * 2;
    brightness = max(static_cast<int16_t>(0), brightness); // Ensure brightness is not negative

    leds[i] = color;
    leds[i].fadeToBlackBy(255 - brightness);
  }

  // Move the "wave" position for the next frame
  position += speed;
  if (position >= 256) position -= 256; // Wrap position
}
