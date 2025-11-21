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

// Defines the pin number for the LED that serves as an indicator when the chain toggle button is pressed.
// Adjust this pin number to suit the configuration of your circuit.
// - Arduino: Connect the indicator LED to pin 5 and set CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN to 5.
// - D1 Mini: Connect the indicator LED to the pin labeled "D5" (which corresponds to GPIO 14) and set CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN to 13.
// - Generic ESP8266 boards: Connect the indicator LED to GPIO 13 and set CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN to 14.
#if defined(BOARD_ESP8266)
  #define DATA_PIN 14
#elif defined(BOARD_ARDUINO)
  #define DATA_PIN 5
#elif defined(BOARD_STM32F103C8)
  #define DATA_PIN PA7
#endif


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

// Define the pin number for the button connection based on the board being used.
// - Arduino: Connect the button to pin 2 and define DESIGN_BUTTON_PIN as 2.
// - D1 Mini: Connect the button to pin D1 and define DESIGN_BUTTON_PIN as 5, corresponding to GPIO5.
// - Generic ESP8266 boards: Connect the button to GPIO 5 and define DESIGN_BUTTON_PIN as 5.
#if defined(BOARD_ARDUINO)
  #define DESIGN_BUTTON_PIN 2
#elif defined(BOARD_STM32F103C8)
  #define DESIGN_BUTTON_PIN PB8
#elif defined(BOARD_ESP8266)
  #define DESIGN_BUTTON_PIN 5
#endif

// Delay before committing the final user-selected design to EEPROM, providing a window to capture the last
// design change in case of rapid user interactions. This approach minimizes EEPROM writes by waiting for
// user input to stabilize, thereby extending EEPROM lifespan by reducing write cycles.
#define EEPROM_WRITE_DELAY_AFTER_LAST_CHANGE 2500

// Choose between LED_ACTIVE_LOW and LED_ACTIVE_HIGH based on your LED's configuration.
// Define LED_ACTIVE_LOW for using common anode RGB LEDs or active-low LEDs.
// Or if you have +ve connected to the LED and you want to toggle the -ve, use LED_ACTIVE_LOW.
// This setting affects how the LED is turned on or off in the code.
// Comment this line for common cathode RGB LEDs or active-high LEDs.
#define LED_ACTIVE_LOW

// Defines the pin number for the LED used as an indicator when the button is pressed.
// Ensure this value matches the physical connection in your setup.
// - Arduino: Connect the indicator LED to pin 8 and set BUTTON_DESIGN_LED_INDICATOR_PIN to 8.
// - D1 Mini: Connect the indicator LED to pin labeled "D6" (which corresponds to GPIO 12) and set BUTTON_DESIGN_LED_INDICATOR_PIN to 12.
// - Generic ESP8266 boards: Connect the indicator LED to GPIO 12 and set BUTTON_DESIGN_LED_INDICATOR_PIN to 12.
#if defined(BOARD_ARDUINO)
  #define BUTTON_DESIGN_LED_INDICATOR_PIN 8
#elif defined(BOARD_STM32F103C8)
  #define BUTTON_DESIGN_LED_INDICATOR_PIN PC13
#elif defined(BOARD_ESP8266)
  #define BUTTON_DESIGN_LED_INDICATOR_PIN 12
#endif


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
  // Defines the pin number for the LED chain toggle button based on the board being used.
  // Ensure this value matches the physical connection on your hardware.
  // - Arduino: Connect the LED chain toggle button to pin 3 and define LED_CHAIN_TOGGLE_BUTTON_PIN as 3.
  // - D1 Mini: Connect the LED chain toggle button to the pin labeled "D2" (corresponding to GPIO 4) and define LED_CHAIN_TOGGLE_BUTTON_PIN as 4.
  // - Generic ESP8266 boards: Connect the LED chain toggle button to GPIO 4 and define LED_CHAIN_TOGGLE_BUTTON_PIN as 4.
  #if defined(BOARD_ARDUINO)
    #define LED_CHAIN_TOGGLE_BUTTON_PIN 3
  #elif defined(BOARD_STM32F103C8)
    #define LED_CHAIN_TOGGLE_BUTTON_PIN PB9
  #elif defined(BOARD_ESP8266)
    #define LED_CHAIN_TOGGLE_BUTTON_PIN 4
  #endif

  // Defines the maximum number of LEDs that can be controlled in the chain.
  // Adjust this value based on the maximum number of LEDs your setup can support.
  #define MAX_LED_COUNT 100

  // Defines the pin number for the LED used as an indicator when the chain toggle button is pressed.
  // Change this pin number according to your circuit's design.
  // On Arduino, set BUTTON_DESIGN_LED_INDICATOR_PIN to 7, connect LED to Pin 7
  // On D1 mini, set BUTTON_DESIGN_LED_INDICATOR_PIN to 13, connect LED to Pin D7
  // On bare ESP8266, set BUTTON_DESIGN_LED_INDICATOR_PIN to 13, connect LED to GPIO 13
  #if defined(BOARD_ARDUINO)
    #define CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN 7
  #elif defined(BOARD_STM32F103C8)
    #define CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN PA8
  #elif defined(BOARD_ESP8266)
    #define CHAIN_TOGGLE_BUTTON_LED_INDICATOR_PIN 13
  #endif

  // Defines the EEPROM address used to store the LED count.
  // This address is used to save and retrieve the number of LEDs in the chain across restarts.
  #define EEPROM_LED_COUNT_ADDRESS_1 4
  #define EEPROM_LED_COUNT_ADDRESS_2 5
  #define EEPROM_LED_COUNT_ADDRESS_3 6
  #define EEPROM_LED_COUNT_ADDRESS_4 7
#endif
