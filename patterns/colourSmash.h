DEFINE_GRADIENT_PALETTE(brightHeatMap_gp_colourSmash) {
  0x0, 0xff, 0x55, 0x55,
  0x7f, 0x55, 0x55, 0xff,
  0xbf, 0x3c, 0xe3, 0xb5,
  0xff, 0xff, 0xff, 0x0,
} ;

extern const CRGBPalette16 brightHeatMapColourSmash = brightHeatMap_gp_colourSmash ;
unsigned int colourIndex[NUM_LEDS] ;

void initColourSmash() {
  for (unsigned char i = 0 ; i < NUM_LEDS ; ++i) {
    colourIndex[i] = random8() ;
  }
}

void colourSmash() {
  for (unsigned int i = 0 ; i < NUM_LEDS ; ++i) {
    leds[i] = ColorFromPalette(brightHeatMapColourSmash, colourIndex[i], 0xff) ;
  }

  EVERY_N_MILLISECONDS(5) {
    for (unsigned int i = 0 ; i < NUM_LEDS ; ++i) {
      if (colourIndex[i] == 0xff) {
        colourIndex[i] = 0 ;
      } else {
        colourIndex[i]++ ;
      }
    }
  }

  FastLED.show() ;
}
