void plainColour(unsigned long colour) {
	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i)
		leds[i] = colour ;

	FastLED.show() ;
}
