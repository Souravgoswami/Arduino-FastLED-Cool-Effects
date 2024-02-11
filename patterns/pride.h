// Pride2015
// Animated, ever-changing rainbows.
// by Mark Kriegsman
// Minor Mods by Sourav Goswami (2021)

void pride(uint16_t numLED) {
  uint8_t sat8, brightdepth, msmultiplier;
  uint16_t brightnessthetainc16;
  uint16_t hue16, hueinc16;
  uint32_t ms, deltams;
  uint16_t b16, bri16;
  uint8_t bri8;
  uint8_t hue8;
  uint16_t brightnesstheta16;
  uint16_t i;

  static uint32_t sPseudotime = 0;
  static uint32_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  sat8 = beatsin88( 87, 220, 250);
  brightdepth = beatsin88( 341, 96, 224);
  brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  msmultiplier = beatsin88(147, 23, 60);

  hue16 = sHue16;
  hueinc16 = beatsin88(113, 1, 3000);

  ms = millis();
  deltams = ms - sLastMillis;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  brightnesstheta16 = sPseudotime;

  for(i = 0; i < numLED; ++i) {
    hue16 += hueinc16;
    hue8 = hue16 >> 8;

    brightnesstheta16  += brightnessthetainc16;
    b16 = sin16(brightnesstheta16) + 32768;

    bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) >> 16;
    bri8 = ((uint32_t)(((uint32_t)bri16) * brightdepth) >> 16) + (255 - brightdepth);

    nblend(leds[numLED - 1 - i], (CRGB)CHSV(hue8, sat8, bri8), 128);
  }

  FastLED.show();
}
