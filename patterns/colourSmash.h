DEFINE_GRADIENT_PALETTE(brightHeatMap_gp_colourSmash) {
  0x00, 0xff, 0x00, 0x00, // Red
  0x20, 0xff, 0x14, 0x93, // Hot Pink
  0x40, 0x00, 0x80, 0x80, // Teal
  0x60, 0x00, 0xff, 0x00, // Green
  0x80, 0xff, 0x00, 0xff, // Magenta
  0xA0, 0xff, 0x8c, 0x00, // Orange
  0xC0, 0xff, 0xff, 0x00, // Yellow
  0xE0, 0x4b, 0x00, 0x82, // Indigo
  0xff, 0x00, 0xff, 0xff  // Cyan
};

DEFINE_GRADIENT_PALETTE(brightHeatMap_gp_colourSmash2) {
  0x00, 0xff, 0x00, 0x00, // Red
  0x10, 0xee, 0x82, 0xee, // Violet
  0x20, 0xff, 0x8c, 0x00, // Dark Orange
  0x40, 0x00, 0x80, 0x00, // Olive Green
  0x50, 0x00, 0xbf, 0xff, // Deep Sky Blue
  0x60, 0x00, 0xff, 0xff, // Aqua
  0x70, 0x00, 0x64, 0x00, // Dark Green
  0x80, 0xff, 0x00, 0xff, // Magenta
  0x90, 0x8a, 0x2b, 0xe2, // Blue Violet
  0xa0, 0xff, 0x45, 0x00, // Orange Red
  0xb0, 0x7c, 0xfc, 0x00, // Lawn Green
  0xc0, 0xad, 0xd8, 0xe6, // Light Blue
  0xd0, 0xff, 0x14, 0x93, // Deep Pink
  0xe0, 0xcd, 0x5c, 0x5c, // Indian Red
  0xf0, 0x4b, 0x00, 0x82, // Indigo
  0xff, 0x00, 0xff, 0xff  // Cyan
};

struct ColourSmashData {
  bool colourSmashInitialized = false;
  const CRGBPalette16 brightHeatMapColourSmash = brightHeatMap_gp_colourSmash;
  const CRGBPalette16 brightHeatMapColourSmash2 = brightHeatMap_gp_colourSmash2;
  uint16_t colourIndex[MAX_LED_COUNT];
};

struct ColourSmashData colourSmashData;

void runColourSmash(CRGBPalette16 palette, uint8_t numLED, int16_t everyNMilliseconds) {
  if (!colourSmashData.colourSmashInitialized) {
    for (uint8_t i = 0; i < numLED; ++i) {
      colourSmashData.colourIndex[i] = generateRandomBrightColour();
    }
    colourSmashData.colourSmashInitialized = true;
  }

  for (uint8_t i = 0; i < numLED; ++i) {
    CRGBPalette16 pal = palette;
    leds[i] = ColorFromPalette(pal, colourSmashData.colourIndex[i], 0xff);
  }

  EVERY_N_MILLISECONDS(everyNMilliseconds) {
    for (uint8_t i = 0; i < numLED; ++i) {
      colourSmashData.colourIndex[i] = (colourSmashData.colourIndex[i] + 1) % 256;
    }
  }
}

void colourSmash(uint8_t numLED) {
  runColourSmash(colourSmashData.brightHeatMapColourSmash, numLED, 5);
}

void colourSmash2(uint8_t numLED) {
  runColourSmash(colourSmashData.brightHeatMapColourSmash2, numLED, 20);
}
