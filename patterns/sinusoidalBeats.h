void sinusoidalBeats() {
  unsigned int sinBeatPos1 = beatsin16(30, 0, NUM_LEDS - 1, 0, 0) ;
  unsigned int sinBeatPos2 = beatsin16(30, 0, NUM_LEDS - 1, 0, 0) ;

  unsigned char sinBeatCol1 = beatsin8(45, 0, 255, 0, 0) ;
  unsigned char sinBeatCol2 = beatsin8(60, 0, 255, 0, 0) ;

  fill_gradient(
    leds,
    sinBeatPos1,
    CHSV(sinBeatCol1, 255, 255),
    sinBeatPos2,
    CHSV(sinBeatCol2, 255, 255)
  ) ;

  FastLED.show() ;
}
