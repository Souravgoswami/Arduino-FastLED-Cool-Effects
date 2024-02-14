#define BOARD_ARDUINO
// #define BOARD_ESP8266

// IMPORTANT NOTE regarding the use of FASTLED_ALL_PINS_HARDWARE_SPI:
//
// The FASTLED_ALL_PINS_HARDWARE_SPI directive is intended to force the FastLED library
// to use hardware SPI for communication with LED strips. However, its applicability and
// benefits depend significantly on the type of LED strip being used, as follows:
//
// DO NOT USE `FASTLED_ALL_PINS_HARDWARE_SPI` WITH:
// - WS2813: A single-wire, timing-specific protocol LED strip.
// - SK6812 (including RGBW variants): Similar to WS2812 but includes an optional white channel, requiring precise timing.
// - NeoPixel: Brand name by Adafruit for individually addressable RGB LEDs, which use timing-specific protocols.
// - APA104/APA106: These strips also rely on precise timing protocols similar to WS281x series.
// - TM1809/TM1804: Featuring a single-wire protocol that requires specific timing sequences.
//
// These LED types require precise timing control for their data signals, which is typically achieved through
// software "bit-banging" rather than using the hardware SPI. Using `FASTLED_ALL_PINS_HARDWARE_SPI` with these
// LEDs can lead to communication issues, such as flickering or incorrect colors, because they do not communicate
// using the standard SPI protocol.
//
// CAN USE `FASTLED_ALL_PINS_HARDWARE_SPI` WITH:
// - APA102/DotStar: These LED strips use a standard SPI protocol, allowing for the use of hardware SPI
//   for faster data transmission and potentially smoother animations.
// - LPD8806: Another LED strip type that communicates over a standard SPI protocol, benefiting from
//   hardware SPI's increased performance and reduced CPU load.
//
// If you are working with APA102/DotStar or LPD8806 LED strips, defining `FASTLED_ALL_PINS_HARDWARE_SPI`
// can enhance performance by utilizing the microcontroller's hardware SPI module. For all other mentioned
// LED types, it is recommended to avoid this definition to ensure proper operation and visual output.
// #define FASTLED_ALL_PINS_HARDWARE_SPI
