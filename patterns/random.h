void generateRandomColours(unsigned long *ary) {
	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i) {
		unsigned long red = rand() & 255 ;
		unsigned long green = rand() & 255 ;
		unsigned long blue = rand() & 255 ;

		unsigned long colour = (red << 16) + (green << 8) + blue ;
		ary[i] = colour ;
	}
}

void randomColoursDelayed(unsigned long *ary) {
	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i) {
		leds[i] = ary[i] ;
		FastLED.show() ;
		delay(20) ;
	}
}

void randomColours(unsigned long *ary) {
	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i) {
		leds[i] = ary[i] ;
	}

	FastLED.show() ;
}

void setBrightColours(const unsigned long *ary, unsigned char length) {
	unsigned char colIndex = 0 ;

	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i) {
		if (colIndex == length) colIndex = 0 ;
		leds[i] = ary[colIndex] ;
		colIndex += 1 ;
	}

	FastLED.show() ;
}

void rotateBrightColours(unsigned long *ary, unsigned char length) {
	unsigned char colIndex = 0 ;

	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i) {
		if (colIndex == length) colIndex = 0 ;
		leds[i] = ary[colIndex] ;
		colIndex += 1 ;

		delay(10) ;
		FastLED.show() ;

		arrayRotate(ary, length) ;
	}
}
