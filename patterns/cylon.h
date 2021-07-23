void fadeall() {
	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i) leds[i].nscale8(250) ;
}

void cylon() {
	static unsigned char hue = 0 ;

	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i) {
		leds[i] = CHSV(hue++, 255, 255) ;
		FastLED.show() ;
		fadeall() ;
		delay(10) ;
	}

	for(unsigned char i = (NUM_LEDS) - 1 ; i ; --i) {
		leds[i] = CHSV(hue++, 255, 255) ;
		FastLED.show() ;
		fadeall() ;
		delay(10) ;
	}
}
