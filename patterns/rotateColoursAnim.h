void arrayRotate(unsigned long *ary, unsigned char length) {
	length-- ;
	unsigned long first = ary[0] ;

	for(unsigned char i = 0 ; i < length ; ++i)
		ary[i] = ary[i + 1] ;

	ary[length] = first ;
}

void rotateColoursAnim(unsigned long *colours, unsigned char length) {
	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i) {
		leds[i] = colours[0] ;
		arrayRotate(colours, length) ;
		FastLED.show() ;
		delay(10) ;
	}
}
