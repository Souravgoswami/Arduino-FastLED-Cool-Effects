# Arduino-FastLED-Cool-Effects
Combines cool FastLED effects of arduino, that's available by clicking the interrupt button.

This code tested on both an Arduino Uno and Arduino Nano.

# Setup:
To setup, you need the following parts:
1. 1xArduino Uno / Nano.
2. A chain of WS2811 fully addressable RGB LED. You can tweak the number in the Code.
3. Some wires.
4. 1x10KΩ Resistor.
5. A momentary push button.

You then need to connect the parts in the following way:

![WS2811](https://raw.githubusercontent.com/Souravgoswami/Arduino-FastLED-Cool-Effects/master/images/circuit.jpg)

To change any configuration, modify the userConfig.h file.
To change board from Arduino to ESP8266 or D1 mini, modify the board.h file.

To change LED modes, press the momentary switch button!

You can use a different external power supply for the LED, while the arduino runs on another. In that case, the + and - will connect to the external power supply, and the ground (-) should be attached to the arduino ground \[the + should only go to the LED, it shouldn't touch the arudino].
