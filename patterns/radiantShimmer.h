DEFINE_GRADIENT_PALETTE(brightHeatMap_gp_radiantShimmer) {
  0, 255, 0, 0,      // Bright Red
  43, 255, 128, 0,   // Bright Orange
  85, 255, 255, 0,   // Bright Yellow
  128, 0, 255, 0,    // Bright Green
  170, 0, 255, 255,  // Bright Cyan
  212, 0, 0, 255,    // Bright Blue
  255, 128, 0, 255,  // Bright Purple
};

extern const CRGBPalette16 brightHeatMapRadiantShimmer = brightHeatMap_gp_radiantShimmer;

void radiantShimmer(short numLED) {
  unsigned char sinBeatPos1 = beatsin16(15, 0, numLED - 1); // Adjusting speed
  unsigned char sinBeatPos2 = beatsin16(45, 0, numLED - 1); // Creating more dynamic changes

  fill_palette(leds, numLED, (sinBeatPos1 + sinBeatPos2) / 2, 8, brightHeatMapRadiantShimmer, 255, LINEARBLEND);

  FastLED.show();
}
