struct FallingSnowData {
  uint8_t chanceOfNewFlake = 2; // Initial chance of a new snowflake
  uint32_t lastChangeTime = 0;
  uint16_t  stormDuration = 5000; // Duration of each storm phase in milliseconds
  uint16_t currentHue = 0; // Track the current hue for rotation
  bool calmPhase = false; // Indicates whether the system is currently in a calm phase
};

FallingSnowData fallingSnowData;

void fallingRainbowSnowEffect(CRGB leds[], uint8_t numLEDs) {
  uint32_t currentTime = millis();
  if (currentTime > fallingSnowData.lastChangeTime + fallingSnowData.stormDuration) {
    // Randomly choose the storm's intensity phase
    if (random(0, 2) == 0) {
      // Calm phase
      fallingSnowData.chanceOfNewFlake = 2;
      fallingSnowData.stormDuration = random(5000, 10001);
      fallingSnowData.calmPhase = true;
    } else {
      // Intense phase
      fallingSnowData.chanceOfNewFlake = random(1, 3);
      fallingSnowData.stormDuration = random(2000, 5000);
      fallingSnowData.calmPhase = false;
    }
    fallingSnowData.lastChangeTime = currentTime;
  }

  // Dim all LEDs slightly to give the effect of falling
  for (uint8_t i = 0; i < numLEDs; i++) {
    leds[i].fadeToBlackBy(10);
  }

  uint8_t fallSpeed = random(1, 8);

  // Randomly add new "snowflakes" and calculate their color based on position
  if (random(0, fallingSnowData.chanceOfNewFlake) == 0) {
    uint8_t flakePos = random8(numLEDs);
    uint8_t hue = (fallingSnowData.currentHue + map(flakePos, 0, numLEDs - 1, 0, 256)) % 256;

    if (fallingSnowData.calmPhase) {
      fallingSnowData.currentHue = (fallingSnowData.currentHue + 1) % 256;
    }

    leds[flakePos] = CHSV(hue, 255, 255);
  }

  FastLED.show();
  FastLED.delay(fallSpeed);
}
