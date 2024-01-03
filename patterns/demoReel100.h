// List of patterns to cycle through.  Each is defined as a separate function below.
struct DemoReel100Data {
	unsigned char gCurrentPatternNumber = 0;
	unsigned short fps = 120;
	unsigned char gHue = 0; // rotating "base color" used by many of the patterns
} __attribute__((packed));

DemoReel100Data demoReel100Data;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void rainbow(short numLEDs) {
	// FastLED's built-in rainbow generator
	fill_rainbow( leds, numLEDs, demoReel100Data.gHue, 7);
}

void addGlitter(fract8 chanceOfGlitter, int numLEDs) {
	if( random8() < chanceOfGlitter) {
		leds[random16(numLEDs)] += CRGB::White;
	}
}

void rainbowWithGlitter(short numLEDs) {
	// built-in FastLED rainbow, plus some random sparkly glitter
	rainbow(numLEDs);
	addGlitter(80, numLEDs);
}

void confetti(short numLEDs) {
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy( leds, numLEDs, 10);
	int pos = random16(numLEDs);
	leds[pos] += CHSV( demoReel100Data.gHue + random8(64), 200, 255);
}

void juggle(short numLEDs) {
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy( leds, numLEDs, 20);
	byte dothue = 0;
	for(unsigned char i = 0; i < 8; ++i) {
		leds[beatsin16( i+7, 0, numLEDs-1 )] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}

typedef void (*SimplePatternList[])(short numLED);
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, juggle };

void nextPattern() {
	// add one to the current pattern number, and wrap around at the end
	demoReel100Data.gCurrentPatternNumber = (demoReel100Data.gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void demoReel100(short numLED) {
	gPatterns[demoReel100Data.gCurrentPatternNumber](numLED);
	FastLED.show();

	// insert a delay to keep the framerate modest
	FastLED.delay(1000 / demoReel100Data.fps);

	// do some periodic updates
	EVERY_N_MILLISECONDS( 20 ) { demoReel100Data.gHue++; } // slowly cycle the "base color" through the rainbow
	EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}
