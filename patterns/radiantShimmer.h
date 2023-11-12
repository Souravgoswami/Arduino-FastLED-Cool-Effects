DEFINE_GRADIENT_PALETTE(brightHeatMap_gp_radiantShimmer) {
  0, 255, 85, 85,
  43, 255, 80, 166,
  85, 85, 85, 255,
  128, 60, 227, 181,
  170, 180, 120, 255,
  212, 255, 255, 0,
  255, 80, 80, 255,
} ;

extern const CRGBPalette16 brightHeatMapRadiantShimmer = brightHeatMap_gp_radiantShimmer ;

void radiantShimmer() {
  unsigned char sinBeatPos1 = beatsin16(30, 0, NUM_LEDS - 1, 0, 0) ;
  unsigned char sinBeatPos2 = beatsin16(90, 0, NUM_LEDS - 1, 0, 0) ;

  unsigned char sinBeatCol = beatsin8(60, 0, 255, 0, 0) ;
  fill_palette(leds, NUM_LEDS, (sinBeatPos1 + sinBeatPos2) / 2, 10, brightHeatMapRadiantShimmer, 255, LINEARBLEND) ;

  FastLED.show() ;
}
