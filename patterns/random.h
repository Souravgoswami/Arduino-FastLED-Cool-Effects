// Reject previous colour in LED.
// For example:
//	Invalid: g - g - r - b - b - g
//	Valid: g - r - b - r - b - g
void arrayShuffleUniqColoursEffect(const uint32_t *brightColours, uint8_t length, CRGB *dest, uint8_t dest_length) {
  if (length > 1) {
    uint32_t lastColor = 0xFFFFFFFF; // An unlikely color value

    for(uint8_t i = 0; i < dest_length; ++i) {
      uint32_t newColor;
      uint8_t attempts = 0;
      do {
        newColor = brightColours[random8(length)];
        attempts++;
        // Fallback strategy after 10 attempts
        if (attempts > 10) {
          // Find a color that is different from the last color
          for (uint8_t j = 0; j < length; ++j) {
            if (brightColours[j] != lastColor) {
              newColor = brightColours[j];
              break;
            }
          }
          break;
        }
      } while(newColor == lastColor);

      dest[i] = CRGB(newColor); // Assuming CRGB accepts a uint32_t color value
      lastColor = newColor;
    }
  }

  FastLED.show();
}

void rotateBrightColours(uint32_t *ary, uint8_t length, uint16_t numLED) {
  uint8_t colIndex = 0;

  for(uint16_t i = 0; i < numLED; ++i) {
    if (colIndex == length) colIndex = 0;
    leds[i] = ary[colIndex];
    colIndex += 1;
    FastLED.delay(10);
    FastLED.show();
  }

  arrayRotate(ary, length);
}

void fadeBetweenColors(CRGB *fromColors, CRGB *toColors, uint8_t length, uint16_t numLEDs, uint16_t fadeDuration) {
  constexpr uint8_t stepSize = 10;

  for (int16_t step = 0; step <= 100; step += stepSize) {
    for (uint8_t i = 0; i < numLEDs; ++i) {
      uint8_t fromIndex = i % length;
      CRGB fromColor = fromColors[fromIndex];
      CRGB toColor = toColors[fromIndex];
      leds[i] = blend(fromColor, toColor, step);
    }

    FastLED.show();
    FastLED.delay(fadeDuration / 100);
  }
}

void rotateBrightColoursSmooth(uint32_t *ary, uint8_t length, uint16_t numLED) {
  CRGB toColours[numLED];
  uint8_t colIndex = 0;
  constexpr uint32_t fadeDurationFactor = 125000;
  uint16_t fadeDuration = fadeDurationFactor / numLED;

  for(uint16_t i = 0; i < numLED; ++i) {
    toColours[i] = ary[colIndex];
    colIndex = (colIndex + 1) % length;
  }

  fadeBetweenColors(leds, toColours, length, numLED, fadeDuration);
  arrayRotate(ary, length);
}
