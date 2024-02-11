void sinusoidalBeats(uint16_t numLED) {
  uint16_t sinBeatPos1 = beatsin16(30, 0, numLED - 1, 0, 0);
  uint16_t sinBeatPos2 = beatsin16(30, 0, numLED - 1, 0, 0);

  uint8_t sinBeatCol1 = beatsin8(45, 0, 255, 0, 0);
  uint8_t sinBeatCol2 = beatsin8(60, 0, 255, 0, 0);

  fill_gradient(
    leds,
    sinBeatPos1,
    CHSV(sinBeatCol1, 255, 255),
    sinBeatPos2,
    CHSV(sinBeatCol2, 255, 255)
  );
}
