/**
 * Creates a smooth, continuously morphing rainbow effect across an LED strip.
 * This function generates a dynamic rainbow pattern that subtly shifts in color and brightness over time,
 * creating a visually captivating display. It utilizes a combination of linear and sinusoidal interpolations
 * to blend hues and morph the colors smoothly, ensuring a seamless transition throughout the LED strip.
 *
 * @param wait The delay in milliseconds between each frame, controlling the speed of the animation.
 *             A lower value results in a faster animation, while a higher value slows it down.
 * @param numLEDs The number of LEDs in the strip. This function dynamically adjusts to accommodate
 *                any strip length, ensuring the pattern scales correctly across different sizes.
 *
 * Usage Example:
 *    rainbowWave(20, NUM_LEDS); // Call this in the main loop to animate the rainbow wave with a 20ms frame delay.
 */
void rainbowWave(uint8_t wait, uint16_t numLEDs) {
  static uint8_t hue = 0;
  uint8_t time = millis() / 25;

  for(uint8_t i = 0; i < numLEDs; i++) {
    // Calculate the base hue with a smooth transition
    uint8_t baseHue = (i * 10) + hue;

    // Calculate the morphing hue using a sine wave for smooth transitions
    uint8_t morphHue = baseHue + sin8(time);

    // Blend the base color and the morph color for a smooth morph effect
    CRGB baseColor = CHSV(baseHue, 255, 255);
    CRGB morphColor = CHSV(morphHue, 255, 255);
    leds[i] = blend(baseColor, morphColor, 128);
  }

  hue++;
  FastLED.show();
  FastLED.delay(wait);
}
