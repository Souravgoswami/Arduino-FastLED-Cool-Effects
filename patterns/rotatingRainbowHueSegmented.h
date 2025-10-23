void rotatingRainbowHueSegmented(uint8_t numLED, uint16_t ledsPerSegment) {
  static uint8_t defaultStartHue = 0;

  for (uint8_t i = 0; i < numLED; i += ledsPerSegment) {
    uint16_t segmentLength = min(ledsPerSegment, numLED - i);
    uint8_t startHue = defaultStartHue + (i / ledsPerSegment) * 16;
    uint8_t deltaHue = 255 / segmentLength;

    fill_rainbow(&leds[i], segmentLength, startHue, deltaHue);
  }

  defaultStartHue++;
}
