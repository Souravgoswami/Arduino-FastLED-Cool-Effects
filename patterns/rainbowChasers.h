enum RainbowChasersGroupState { OFF, FADING_IN, ON, FADING_OUT };

struct RainbowChasersGroup {
  RainbowChasersGroupState state;
  int brightness;
};

RainbowChasersGroup groups[3] = {
  {FADING_IN, 0}, // Ensure at least one group starts the fade-in process
  {OFF, 0},
  {OFF, 0}
};

// Timing and fading parameters
struct RainbowChasersData {
  unsigned long lastUpdateTime = 0;
  const unsigned char updateInterval = 14; // Milliseconds between updates for smooth transition
  const unsigned char fadeAmount = 5; // Adjust for faster or slower fade
  unsigned char hueOffset = 0; // Starting offset color
};

RainbowChasersData rainbowChasersData;


void updateRainbowChasersGroupState(RainbowChasersGroup &group) {
  switch (group.state) {
    case FADING_IN:
      group.brightness += rainbowChasersData.fadeAmount;
      if (group.brightness >= 255) {
        group.brightness = 255;
        group.state = ON;
      }
      break;
    case FADING_OUT:
      group.brightness -= rainbowChasersData.fadeAmount;
      if (group.brightness <= 0) {
        group.brightness = 0;
        group.state = OFF;
      }
      break;
    default:
      break;
  }
}

void staticRainbowChase(CRGB* leds, int numLEDs) {
  unsigned long currentTime = millis();
  if (currentTime - rainbowChasersData.lastUpdateTime > rainbowChasersData.updateInterval) {
    rainbowChasersData.lastUpdateTime = currentTime;

    // Update each group's state and brightness
    for (int i = 0; i < 3; i++) {
      updateRainbowChasersGroupState(groups[i]);
    }

    // Check if it's time to rotate the states
    if (groups[0].state == ON && groups[1].state == OFF) {
      groups[0].state = FADING_OUT;
      groups[1].state = FADING_IN;
    } else if (groups[1].state == ON && groups[2].state == OFF) {
      groups[1].state = FADING_OUT;
      groups[2].state = FADING_IN;
    } else if (groups[2].state == ON && groups[0].state == OFF) {
      groups[2].state = FADING_OUT;
      groups[0].state = FADING_IN;
    }

    // Apply the brightness and color to the LEDs
    for (int i = 0; i < numLEDs; i++) {
      int groupIndex = i % 3; // Determine the group of the current LED

      // Calculate a static hue value for each LED to create a stationary rainbow pattern across the LED strip.
      // Assign the calculated hue with full saturation and group-specific brightness to the LED.
      byte hue = (i * 255 / numLEDs) % 255;
      CHSV hsvColor = CHSV(hue, 255, groups[groupIndex].brightness);
      leds[i] = hsvColor;
    }

    FastLED.show();
  }
}

void dynamicRainbowChase(CRGB* leds, int numLEDs) {
  unsigned long currentTime = millis();
  if (currentTime - rainbowChasersData.lastUpdateTime > rainbowChasersData.updateInterval) {
    rainbowChasersData.lastUpdateTime = currentTime;

    // Update each group's state and brightness
    for (int i = 0; i < 3; i++) {
      updateRainbowChasersGroupState(groups[i]);
    }

    rainbowChasersData.hueOffset += 1;

    // Check if it's time to rotate the states
    if (groups[0].state == ON && groups[1].state == OFF) {
      groups[0].state = FADING_OUT;
      groups[1].state = FADING_IN;
    } else if (groups[1].state == ON && groups[2].state == OFF) {
      groups[1].state = FADING_OUT;
      groups[2].state = FADING_IN;
    } else if (groups[2].state == ON && groups[0].state == OFF) {
      groups[2].state = FADING_OUT;
      groups[0].state = FADING_IN;
    }

    // Apply the brightness and color to the LEDs
    for (int i = 0; i < numLEDs; i++) {
      int groupIndex = i % 3; // Determine the group of the current LED

    // Calculate hue for each LED with an offset for creating a moving rainbow effect across the LED strip.
    // Assign the calculated hue with full saturation and group-specific brightness to the LED.
      byte hue = (rainbowChasersData.hueOffset + i * 255 / numLEDs) % 255;
      CHSV hsvColor = CHSV(hue, 255, groups[groupIndex].brightness);
      leds[i] = hsvColor;
    }

    FastLED.show();
  }
}
