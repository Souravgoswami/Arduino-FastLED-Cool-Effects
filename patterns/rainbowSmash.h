// Define the structure for holding the rainbow smash effect data
struct RainbowSmashData {
  // delayConst is calculated to maintain a consistent animation speed regardless of the number of LEDs.
  // It is inversely proportional to the number of LEDs to ensure uniform animation timing.
  // The formula used is: delayConst = (16.5 * 100) / numLEDs.
  // This results in a delay of 16.5 ms for 100 LEDs, 8.25 ms for 50 LEDs, and 33 ms for 200 LEDs.
  const float delayConst = 1650; // (16.5 * 100) or (1.65 * 1000)
  float sineWaveProgress = 0.0;
  uint8_t hue = 0;
  uint8_t offset = 0;
  const uint8_t maxBrightness = 255;
};

// Create an instance of the structure
RainbowSmashData rainbowSmashData;

// Function to create the rainbow smash effect with fading in and out
void rainbowSmash(uint16_t numLEDs) {
  // Define minimum and maximum offset
  const uint8_t minOffset = numLEDs / 10;
  const uint8_t maxOffset = numLEDs / 5;

  // Calculate sine value and map it to segment length
  float sineValue = sin(rainbowSmashData.sineWaveProgress);
  uint8_t segmentLength = map(sineValue * 100, -100, 100, minOffset, maxOffset);

  // Update sine wave progress and reset if necessary
  rainbowSmashData.sineWaveProgress += 0.1; // Adjust for speed
  if (rainbowSmashData.sineWaveProgress > 2 * PI) {
    rainbowSmashData.sineWaveProgress -= 2 * PI;
  }

  // Gradually fade all LEDs
  fadeToBlackBy(leds, numLEDs, 20);

  // Calculate starting points for the segments
  uint16_t leftStart = rainbowSmashData.offset;
  uint16_t rightStart = numLEDs - segmentLength - rainbowSmashData.offset;

  // Ensure the segments don't overlap or go out of bounds
  if (leftStart + segmentLength > numLEDs / 2) {
    leftStart = max(0, (int)numLEDs / 2 - (int)segmentLength);
  }

  rightStart = constrain(rightStart, numLEDs / 2, numLEDs - segmentLength);

  // Fill the segments with a rainbow pattern
  for (int16_t i = 0; i < segmentLength; i++) {
    uint8_t brightness = rainbowSmashData.maxBrightness * (i + 1) / segmentLength;
    uint8_t inverseBrightness = rainbowSmashData.maxBrightness * (segmentLength - i) / segmentLength;

    if (leftStart + i < numLEDs) {
      leds[leftStart + i] = CHSV(rainbowSmashData.hue + i * 7, 255, brightness);
    }

    if (rightStart + i < numLEDs) {
      leds[rightStart + i] = CHSV(rainbowSmashData.hue + i * 7, 255, inverseBrightness);
    }
  }

  // Update the hue and offset for the next cycle
  rainbowSmashData.hue++;
  rainbowSmashData.offset++;

  // Reset offset if segments collide
  if (rightStart <= leftStart + segmentLength) {
    rainbowSmashData.offset = 0; // Reset offset to start over
  }

  FastLED.show();
  FastLED.delay(rainbowSmashData.delayConst / numLEDs);
}
