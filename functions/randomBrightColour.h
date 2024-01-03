uint32_t generateRandomBrightColour() {
  uint8_t r, g, b;
  uint8_t highValue, midValue, lowValue;

  // Assign high, mid, and low values within a bright range
  highValue = random(180, 256); // Brightest
  midValue = random(100, highValue); // Moderately bright
  lowValue = random(50, midValue); // Lowest brightness, but not too dark

  // Randomly assign these to R, G, and B
  uint8_t colorOrder = random(6);
  switch (colorOrder) {
    case 0:
      r = highValue; g = midValue; b = lowValue; break;
    case 1:
      r = highValue; g = lowValue; b = midValue; break;
    case 2:
      g = highValue; r = midValue; b = lowValue; break;
    case 3:
      g = highValue; r = lowValue; b = midValue; break;
    case 4:
      b = highValue; r = midValue; g = lowValue; break;
    default:
      b = highValue; r = lowValue; g = midValue; break;
  }

  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
