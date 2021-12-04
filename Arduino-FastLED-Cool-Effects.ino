#pragma GCC optimize("Os")

#include "FastLED.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

/*
 * General Config
 */
#define DATA_PIN 5
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS 50
#define BRIGHTNESS 210

// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;

/*
 * demoReel
 */
#define DEMO_REEL_FRAMES_PER_SECOND  120

/*
 *  fire2012
 */
#define FIRE2012_FRAMES_PER_SECOND 60
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

/*
 * XY Matrix
 */
const bool    kMatrixVertical = false;

#define XY_MATRIX_NUM_LEDS (kMatrixWidth * kMatrixHeight)

/*
 *  Button handler
 */
#define BUTTON_MIN_DELAY 500
#define BUTTON_PIN 2

CRGB leds[NUM_LEDS] ;

#include "functions/arrayFunctions.h"

#include "patterns/pride.h"
#include "patterns/cylon.h"
#include "patterns/demoReel100.h"
#include "patterns/fire2012.h"
#include "patterns/xyMatrix.h"
#include "patterns/plainColour.h"
#include "patterns/random.h"

volatile unsigned long prevPressTime = 0 ;
volatile unsigned char design = 0 ;
unsigned long randomColoursAry[NUM_LEDS] ;
unsigned char randomColoursSet = 0 ;

void setup() {
	// Button press event
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPushEvent, CHANGE) ;

 	delay(2000) ;

 	Serial.begin(57600) ;

 	srand(analogRead(A0)) ;

	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
		.setCorrection(TypicalLEDStrip)
		.setDither(BRIGHTNESS < 255) ;

	FastLED.setBrightness(BRIGHTNESS) ;
}

unsigned long colours[] = {
	0xff5010, 0x008080, 0x32CD32,
	0x00ffff, 0xff50d6, 0xff00ff,
	0xffff00, 0xff2200, 0xffffff
} ;

unsigned long brightColours[] = {
	0xff5555, 0x00ff55, 0x00ff22, 0x00aa00,
	0x5555ff, 0x3ce3b4, 0x5555ff, 0xffff00,
	0xff0000, 0xffaa00, 0xff4400
} ;

const unsigned char brightColoursLen = sizeof(brightColours) / sizeof(brightColours[0]) ;

unsigned long leds_ary[NUM_LEDS] ;

void loop() {
	if (design < 3) {
		if (!randomColoursSet) {
			randomColoursSet = 1 ;
			arrayShuffleUniqColours(brightColours, brightColoursLen, leds_ary, NUM_LEDS) ;
		}

		// setBrightColours(brightColours, brightColoursLen) ;
		setBrightColours(leds_ary, NUM_LEDS) ;
	} else if (design < 6) {
		if (!randomColoursSet) {
			randomColoursSet = 1 ;
			arrayShuffle(brightColours, brightColoursLen) ;
		}

		setBrightColours(brightColours, brightColoursLen) ;
	} else if (design == 6) {
		pride() ;		
	} else if (design == 7) {
		cylon() ;
	} else if (design == 8) {
		demoReel100() ;
	} else if (design == 9) {
		fire2012() ;
	} else if (design == 10) {
		xyMatrix() ;
	} else if (design == 11) {
		plainColour(0xff5010) ;
	} else if (design == 12) {
		plainColour(0x008080) ;
	} else if (design == 13) {
		plainColour(0x32cd32) ;
	} else if (design == 14) {
		plainColour(0x00ffff) ;
	} else if (design == 15) {
		plainColour(0xff50d6) ;
	} else if (design == 16) {
		plainColour(0xff00ff) ;
	} else if (design == 17) {
		plainColour(0xffff00) ;
	} else if (design == 18) {
		plainColour(0xff2200) ;
	} else if (design == 19) {
		plainColour(0x00ff00) ;
	} else if (design == 20) {
		plainColour(0x3ce3b4) ;
	} else if (design == 21) {
		plainColour(0xff0011) ;
	} else if (design == 22) {
		generateRandomColours(randomColoursAry) ;
		randomColoursDelayed(randomColoursAry) ;
	}  else if (design > 23 && design < 27) {
		if (!randomColoursSet) {
			generateRandomColours(randomColoursAry) ;
			randomColoursSet = 1 ;
			randomColours(randomColoursAry) ;
		}
	} else if (design == 28) {
		setBrightColours(brightColours, brightColoursLen) ;
	} else {
		rotateBrightColours(brightColours, brightColoursLen) ;
	}
}

void buttonPushEvent() {
	// Minimum delay between switch presses
	if(millis() < prevPressTime) return ;
	prevPressTime = millis() + BUTTON_MIN_DELAY ;
	design = (design + 1) % 30 ;

	randomColoursSet = 0 ;
	FastLED.setBrightness(BRIGHTNESS) ;
}
