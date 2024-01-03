void plainColour(unsigned long colour, int numLED) {
	for(unsigned char i = 0; i < numLED; ++i)
		leds[i] = colour;

	FastLED.show();
}
