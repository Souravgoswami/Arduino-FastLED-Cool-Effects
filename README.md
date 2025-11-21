# Arduino-FastLED-Cool-Effects

A collection of cool FastLED effects for Arduino boards, triggered by pressing the interrupt button.

Supported boards: **Arduino** and **STM32F103C8**.
**ESP8266** also works, though you might notice occasional glitches.

### Tested setups

* **Arduino Uno / Nano:** FastLED v3.10.3
* **STM32F103C8T6:** Board package v2.11.0, FastLED v3.10.2
  *(Note: FastLED v3.10.3 doesn't work properly on STM32F103C8T6.)*

FastLED tends to be a bit unstable on ESP8266, and you may encounter random glitches even when using proper logic-level shifters.

---

## Setup

Connect the components as shown below (for Arduino):

![WS2811](https://raw.githubusercontent.com/Souravgoswami/Arduino-FastLED-Cool-Effects/master/images/circuit.jpg)

For **STM32F103C8**, you'll need a logic-level shifter to convert 3.3 V signals to 5 V.
I've tested the **SN74LV1T34DBVR**, which works great. Other high-speed shifters like **TXB0108** also do the job.

To adjust settings, edit **userConfig.h**.
To switch boards (Arduino / STM32F103C8T6 / ESP8266 based boards), open **boardConfig.h** and uncomment your target board.

Press the **momentary switch button** to change LED modes!

If you're using a separate power supply for the LEDs, connect **+** and **–** to that supply, and make sure the **grounds (–)** of both the LED supply and the Arduino are connected together (common ground).
The **+** line from the LED power supply should **not** be connected to the Arduino.
