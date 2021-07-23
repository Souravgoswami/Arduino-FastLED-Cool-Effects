// List of patterns to cycle through.  Each is defined as a separate function below.
unsigned char gCurrentPatternNumber = 0;
unsigned char gHue = 0; // rotating "base color" used by many of the patterns

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void rainbow() {
	// FastLED's built-in rainbow generator
	fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void addGlitter( fract8 chanceOfGlitter) {
	if( random8() < chanceOfGlitter) {
		leds[ random16(NUM_LEDS) ] += CRGB::White;
	}
}

void rainbowWithGlitter() {
	// built-in FastLED rainbow, plus some random sparkly glitter
	rainbow();
	addGlitter(80);
}

void confetti() {
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy( leds, NUM_LEDS, 10);
	int pos = random16(NUM_LEDS);
	leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon() {
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy( leds, NUM_LEDS, 20);
	int pos = beatsin16( 13, 0, NUM_LEDS-1 );
	leds[pos] += CHSV( gHue, 255, 192);
}

void bpm() {
	// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	uint8_t BeatsPerMinute = 62;
	CRGBPalette16 palette = PartyColors_p;
	uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
	for(unsigned char i = 0; i < NUM_LEDS; i++) { //9948
		leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
	}
}

void juggle() {
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy( leds, NUM_LEDS, 20);
	byte dothue = 0;
	for(unsigned char i = 0 ; i < 8 ; ++i) {
		leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

void nextPattern() {
	// add one to the current pattern number, and wrap around at the end
	gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void demoReel100() {
	gPatterns[gCurrentPatternNumber]();
	FastLED.show();

	// insert a delay to keep the framerate modest
	FastLED.delay(1000 / DEMO_REEL_FRAMES_PER_SECOND);

	// do some periodic updates
	EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
	EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}
