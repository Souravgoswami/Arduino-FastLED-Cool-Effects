// List of patterns to cycle through.  Each is defined as a separate function below.
struct DemoReel100Data {
  uint16_t fps = 120;
  uint8_t gCurrentPatternNumber = 0;
  uint8_t gHue = 0; // rotating "base color" used by many of the patterns
};

DemoReel100Data demoReel100Data;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void rainbow(uint16_t numLEDs) {
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, numLEDs, demoReel100Data.gHue, 7);
}

void addGlitter(fract8 chanceOfGlitter, uint16_t numLEDs) {
  if( random8() < chanceOfGlitter) {
    leds[random16(numLEDs)] += CRGB::White;
  }
}

void rainbowWithGlitter(uint16_t numLEDs) {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow(numLEDs);
  addGlitter(80, numLEDs);
}

void confetti(uint16_t numLEDs) {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, numLEDs, 10);
  uint16_t pos = random16(numLEDs);
  leds[pos] += CHSV( demoReel100Data.gHue + random8(64), 200, 255);
}

void juggle(uint16_t numLEDs) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, numLEDs, 20);
  uint8_t dothue = 0;
  for(uint8_t i = 0; i < 8; ++i) {
    leds[beatsin16( i+7, 0, numLEDs-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

typedef void (*SimplePatternList[])(uint16_t numLED);
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, juggle };

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  demoReel100Data.gCurrentPatternNumber = (demoReel100Data.gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void demoReel100(uint16_t numLED) {
  gPatterns[demoReel100Data.gCurrentPatternNumber](numLED);
  FastLED.show();

  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / demoReel100Data.fps);

  // do some periodic updates
  // slowly cycle the "base color" through the rainbow
  EVERY_N_MILLISECONDS(20) {
    demoReel100Data.gHue++;
  }

  // change patterns periodically
  EVERY_N_SECONDS(10) {
    nextPattern();
  }
}
