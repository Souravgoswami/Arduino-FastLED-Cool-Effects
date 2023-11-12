void sinusoidalBeatsWithBlur2() {
  unsigned char sinBeat1 = beatsin8(30, 0, NUM_LEDS - 1, 0, 0) ;
  unsigned char sinBeat2 = beatsin8(45, 0, NUM_LEDS - 1, 0, 85) ;
  unsigned char sinBeat3 = beatsin8(60, 0, NUM_LEDS - 1, 0, 170) ;

  leds[sinBeat1] = (CRGB) 0x3ce3b4 ;
  leds[sinBeat2] = (CRGB) 0x0000ff ;
  leds[sinBeat3] = (CRGB) 0xff50a6 ;

  EVERY_N_MILLISECONDS(1) {
    for(unsigned char i = 0 ; i < 4 ; ++i) {
      blur1d(leds, NUM_LEDS, 100) ;
    }
  }

  FastLED.show() ;
}
