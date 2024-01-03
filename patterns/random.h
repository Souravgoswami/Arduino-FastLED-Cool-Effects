// Reject previous colour in LED.
// For example:
//	Invalid: g - g - r - b - b - g
//	Valid: g - r - b - r - b - g
void arrayShuffleUniqColoursEffect(const unsigned long *brightColours, unsigned char length, CRGB *dest, unsigned char dest_length) {
  if (length > 1) {
    unsigned long lastColor = 0xFFFFFFFF; // An unlikely color value

    for(unsigned char i = 0; i < dest_length; ++i) {
      unsigned long newColor;
      unsigned char attempts = 0;
      do {
        newColor = brightColours[rand() % length];
        attempts++;
        if (attempts > 10) { // Limit attempts to 10
          newColor = (lastColor + 1) % length; // Fallback strategy
          break;
        }
      } while(newColor == lastColor);

      dest[i] = newColor;
      lastColor = newColor;
    }
  }

  FastLED.show();
}

void rotateBrightColours(unsigned long *ary, unsigned char length, int numLED) {
  unsigned char colIndex = 0;

  for(unsigned char i = 0; i < numLED; ++i) {
    if (colIndex == length) colIndex = 0;
    leds[i] = ary[colIndex];
    colIndex += 1;
    FastLED.delay(10);
    FastLED.show();
  }

  arrayRotate(ary, length);
}

void fadeBetweenColors(CRGB *fromColors, CRGB *toColors, unsigned int length, unsigned short numLEDs, int fadeDuration) {
  int stepSize = 10;

  for (int step = 0; step <= 100; step += stepSize) {
    for (unsigned char i = 0; i < numLEDs; ++i) {
      unsigned char fromIndex = i % length;
      CRGB fromColor = fromColors[fromIndex];
      CRGB toColor = toColors[fromIndex];
      leds[i] = blend(fromColor, toColor, step);
    }

    FastLED.show();
    FastLED.delay(fadeDuration / 100);
  }
}

void rotateBrightColoursSmooth(unsigned long *ary, unsigned char length, int numLED) {
  CRGB toColours[numLED];
  unsigned char colIndex = 0;
  unsigned long fadeDurationFactor = 125000;
  unsigned int fadeDuration = fadeDurationFactor / numLED;

  for(unsigned char i = 0; i < numLED; ++i) {
    toColours[i] = ary[colIndex];
    colIndex = (colIndex + 1) % length;
  }

  fadeBetweenColors(leds, toColours, length, numLED, fadeDuration);
  arrayRotate(ary, length);
}
