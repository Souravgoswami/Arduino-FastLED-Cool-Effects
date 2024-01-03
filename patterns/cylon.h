void cylon(short numLEDs) {
	static unsigned char hue = 0;

	for(unsigned char i = 0; i < numLEDs; ++i) {
		leds[i] = CHSV(hue++, 255, 255);
		FastLED.show();
		for(unsigned char i = 0; i < numLEDs; ++i) leds[i].nscale8(250);
		FastLED.delay(10);
	}

	for(unsigned char i = (numLEDs) - 1; i; --i) {
		leds[i] = CHSV(hue++, 255, 255);
		FastLED.show();
		for(unsigned char i = 0; i < numLEDs; ++i) leds[i].nscale8(250);
		FastLED.delay(10);
	}
}
