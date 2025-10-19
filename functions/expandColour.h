/**
 * Expands a 12-bit RGB color (4 bits per channel) into 24-bit RGB (8 bits per channel).
 *
 * Input format: [ RRRR GGGG BBBB ] in a 16-bit value.
 * Each 4-bit component is duplicated to 8 bits (e.g., 0xA → 0xAA).
 *
 * @param c  12-bit RGB value.
 * @return   24-bit RGB value (0xRRGGBB).
 */
uint32_t expandColour(uint16_t c) {
  uint8_t r = (c >> 8) & 0xF;
  uint8_t g = (c >> 4) & 0xF;
  uint8_t b =  c & 0xF;

  // Duplicate each nibble: 0xF → 0xFF, 0xA → 0xAA, etc.
  r = (r << 4) | r;
  g = (g << 4) | g;
  b = (b << 4) | b;

  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}
