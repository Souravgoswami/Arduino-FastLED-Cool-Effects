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
#define BRIGHTNESS 254

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

#include "patterns/pride.h"
#include "patterns/cylon.h"
#include "patterns/demoReel100.h"
#include "patterns/fire2012.h"
#include "patterns/xyMatrix.h"
#include "patterns/plainColour.h"
#include "patterns/rotateColoursAnim.h"

volatile unsigned long prevPressTime = 0 ;
volatile unsigned char design = 0 ;

void setup() {
 	delay(3000);

 	Serial.begin(57600) ;

	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
		.setCorrection(TypicalLEDStrip)
		.setDither(BRIGHTNESS < 255) ;

	FastLED.setBrightness(BRIGHTNESS) ;

	// Button press event
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPushEvent, CHANGE) ;
}

unsigned long colours[] = {
	0xff5010, 0x008080, 0x32CD32,
	0x00ffff, 0xff50d6, 0xff00ff,
	0xffff00, 0xff2200, 0xffffff
} ;

unsigned long rotateColours[] = {
	0x00ffff, 0x00f6ff, 0x00eeff, 0x00e5ff, 0x00ddff, 0x00d4ff, 0x00ccff, 0x00c3ff, 0x00bbff, 0x00b2ff, 0x00aaff, 0x00a1ff, 0x0099ff, 0x0090ff, 0x0088ff, 0x007fff, 0x0077ff, 0x006eff, 0x0066ff, 0x005dff, 0x0055ff, 0x004cff, 0x0044ff, 0x003bff, 0x0033ff, 0x002aff, 0x0022ff, 0x0019ff, 0x0011ff, 0x0008ff, 0x0000fe, 0x0800f9, 0x1100f3, 0x1900ed, 0x2200e7, 0x2a00e1, 0x3300db, 0x3b00d6, 0x4400d0, 0x4c00ca, 0x5500c4, 0x5d00be, 0x6600b8, 0x6e00b3, 0x7700ad, 0x7f00a7, 0x8800a1, 0x90009b, 0x990095, 0xa10090, 0xaa008a, 0xb20084, 0xbb007e, 0xc30078, 0xcc0072, 0xd4006d, 0xdd0067, 0xe50061, 0xee005b, 0xf60055, 0xff0050, 0xff0055, 0xff005b, 0xff0061, 0xff0067, 0xff006d, 0xff0072, 0xff0078, 0xff007e, 0xff0084, 0xff008a, 0xff0090, 0xff0096, 0xff009b, 0xff00a1, 0xff00a7, 0xff00ad, 0xff00b3, 0xff00b9, 0xff00be, 0xff00c4, 0xff00ca, 0xff00d0, 0xff00d6, 0xff00dc, 0xff00e1, 0xff00e7, 0xff00ed, 0xff00f3, 0xff00f9, 0xff00ff, 0xf608f6, 0xee11ee, 0xe519e5, 0xdd22dd, 0xd42ad4, 0xcc33cc, 0xc33bc3, 0xbb44bb, 0xb24cb2, 0xaa55aa, 0xa15da1, 0x996699, 0x906e90, 0x887788, 0x7f7f7f, 0x778877, 0x6e906e, 0x669966, 0x5da15d, 0x55aa55, 0x4cb24c, 0x44bb44, 0x3bc33b, 0x33cc33, 0x2ad42a, 0x22dd22, 0x19e519, 0x11ee11, 0x08f608, 0x00ff00, 0x00ff08, 0x00ff11, 0x00ff19, 0x00ff22, 0x00ff2a, 0x00ff33, 0x00ff3b, 0x00ff44, 0x00ff4c, 0x00ff55, 0x00ff5d, 0x00ff66, 0x00ff6e, 0x00ff77, 0x00ff7f, 0x00ff88, 0x00ff90, 0x00ff99, 0x00ffa1, 0x00ffaa, 0x00ffb2, 0x00ffbb, 0x00ffc3, 0x00ffcc, 0x00ffd4, 0x00ffdd, 0x00ffe5, 0x00ffee, 0x00fff6
} ;

unsigned char coloursLen = sizeof(rotateColours) / sizeof(rotateColours[0]) ;

void loop() {	
	if (design == 0) {
		pride() ;
	} else if (design == 1) {
		cylon() ;
	} else if (design == 2) {
		demoReel100() ;
	} else if (design == 3) {
		fire2012() ;
	} else if (design == 4) {
		xyMatrix() ;
	} else if (design == 5) {
		plainColour(0xff5010) ;
	} else if (design == 6) {
		plainColour(0x008080) ;
	} else if (design == 7) {
		plainColour(0x32cd32) ;
	} else if (design == 8) {
		plainColour(0x00ffff) ;
	} else if (design == 9) {
		plainColour(0xff50d6) ;
	} else if (design == 10) {
		plainColour(0xff00ff) ;
	} else if (design == 11) {
		plainColour(0xffff00) ;
	} else if (design == 12) {
		plainColour(0xff2200) ;
	} else if (design == 13) {
		plainColour(0x00ff00) ;
	} else if (design == 14) {
		plainColour(0x3ce3b4) ;
	} else if (design == 15) {
		plainColour(0xff0011) ;
	} else {
		rotateColoursAnim(rotateColours, coloursLen) ;	
	}
}

void buttonPushEvent() {
	// Minimum delay between switch presses
	if(millis() < prevPressTime) return ;
	prevPressTime = millis() + BUTTON_MIN_DELAY ;
	design = ++design % 17 ;

	FastLED.setBrightness(BRIGHTNESS) ;
}
