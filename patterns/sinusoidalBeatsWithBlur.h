void sinusoidalBeatsWithBlur() {
  unsigned char sinBeat1 = beatsin8(30, 0, NUM_LEDS - 1, 0, 0) ;
  unsigned char sinBeat2 = beatsin8(30, 0, NUM_LEDS - 1, 0, 42) ;
  unsigned char sinBeat3 = beatsin8(30, 0, NUM_LEDS - 1, 0, 84) ;
  unsigned char sinBeat4 = beatsin8(30, 0, NUM_LEDS - 1, 0, 126) ;
  unsigned char sinBeat5 = beatsin8(30, 0, NUM_LEDS - 1, 0, 168) ;
  unsigned char sinBeat6 = beatsin8(30, 0, NUM_LEDS - 1, 0, 210) ;

  leds[sinBeat1] = (CRGB) 0x00ff00 ;
  leds[sinBeat2] = (CRGB) 0xffff00 ;
  leds[sinBeat3] = (CRGB) 0x3ce3b4 ;
  leds[sinBeat4] = (CRGB) 0x4444ff ;
  leds[sinBeat5] = (CRGB) 0xff50a6 ;
  leds[sinBeat6] = (CRGB) 0x5555ff ;

  EVERY_N_MILLISECONDS(1) {
    for(unsigned char i = 0 ; i < 4 ; ++i) {
      blur1d(leds, NUM_LEDS, 50) ;
    }
  }

  FastLED.show() ;
}
