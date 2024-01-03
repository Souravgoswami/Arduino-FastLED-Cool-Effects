void sinusoidalBeats2(short numLED) {
  unsigned int sinBeatPos1 = beatsin16(30, 0, numLED - 1, 0, 0);
  unsigned int sinBeatPos2 = beatsin16(30, 0, numLED - 1, 0, 0);

  unsigned char sinBeatCol1 = beatsin8(45, 0, 255, 0, 0);
  unsigned char sinBeatCol2 = beatsin8(60, 0, 255, 0, 0);

  fill_gradient(
    leds,
    sinBeatPos1,
    CHSV(sinBeatCol1, 255, 255),
    sinBeatPos2,
    CHSV(sinBeatCol2, 255, 255)
  );

  if(random8() < 50) { // Random chance to create a sparkle
    int pos = random16(numLED); // Pick a random LED
    leds[pos] += CHSV(random8(), 200, 255); // Add a random color with high saturation and brightness
  }

  FastLED.show();
}
