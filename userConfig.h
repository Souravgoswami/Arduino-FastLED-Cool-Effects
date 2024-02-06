/*
 * General Configuration
 *
 * This section contains foundational settings that define the core aspects of the LED display system.
 * These configurations are crucial for the initial setup and proper operation of the LED strip,
 * ensuring that the hardware and software components communicate effectively. Each setting is adjustable
 * to accommodate various types of LED strips, wiring setups, and user preferences.
 * It is essential to tailor these parameters to match your specific hardware configuration
 * and desired operational characteristics.
 */

// Pin number where the LED data line is connected.
#define DATA_PIN 5

// Type of LED strip being used (e.g., WS2811, WS2812B, etc.).
// Change this according to the specific LED strip you are using.
#define LED_TYPE WS2811

// The order of color data transmission (RGB, GRB, etc.).
// This should match the order used by your LED strip.
#define COLOR_ORDER RGB

// Default brightness level of the LEDs (0-255).
// Adjust this value to set the brightness to a comfortable level.
#define BRIGHTNESS 225

/*
 * Button Handler Configuration
 */

// Minimum delay time between button presses in milliseconds.
// This prevents the button from being read multiple times accidentally.
#define BUTTON_MIN_DELAY 500

// Pin number where the button is connected.
#define BUTTON_PIN 2

// Define this for using common anode RGB LEDs or active-low LEDs.
// This setting affects how the LED is turned on or off in the code.
#define LED_ACTIVE_LOW

// Uncomment this line for common cathode RGB LEDs or active-high LEDs.
// Choose between LED_ACTIVE_LOW and LED_ACTIVE_HIGH based on your LED's configuration.
// #define LED_ACTIVE_HIGH

// Specifies the type of LEDs used for the button indicator.
// Change this according to the specific type of your button's LED.
#define BUTTON_LED_TYPE_COMMON_ANODE

// Pin number for the LED used as an indicator when the button is pressed.
// Modify this value to match the physical connection in your setup.
#define BUTTON_PUSH_LED_INDICATOR_PIN 8

// Default number of LEDs to be controlled.
// This value can be adjusted based on the initial setup or preferences.
#define DEFAULT_LED_COUNT 50

// EEPROM address where the design index is stored.
// This is used to save and retrieve the design pattern index across restarts.
#define EEPROM_DESIGN_ADDRESS_1 0
#define EEPROM_DESIGN_ADDRESS_2 1
#define EEPROM_DESIGN_ADDRESS_3 2
#define EEPROM_DESIGN_ADDRESS_4 3

// Uncomment the following line to enable functionality related to the LED chain toggle button.
// This enables additional features and configurations for handling an LED chain.
#define LED_CHAIN_TOGGLE_BUTTON

#ifdef LED_CHAIN_TOGGLE_BUTTON
  // Defines the pin number connected to the LED chain toggle button.
  // Modify this value to match the physical connection on your hardware.
  #define LED_CHAIN_TOGGLE_BUTTON_PIN 3

  // Defines the maximum number of LEDs that can be controlled in the chain.
  // Adjust this value based on the maximum number of LEDs your setup can support.
  #define MAX_LED_COUNT 100

  // Defines the pin number for the LED used as an indicator when the chain toggle button is pressed.
  // Change this pin number according to your circuit's design.
  #define CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN 7

  // Defines the EEPROM address used to store the LED count.
  // This address is used to save and retrieve the number of LEDs in the chain across restarts.
  #define EEPROM_LED_COUNT_ADDRESS_1 4
  #define EEPROM_LED_COUNT_ADDRESS_2 5
  #define EEPROM_LED_COUNT_ADDRESS_3 6
  #define EEPROM_LED_COUNT_ADDRESS_4 7
#endif
