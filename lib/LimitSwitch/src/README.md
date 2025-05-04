# LimitSwitch Library

## Overview

The `LimitSwitch` library provides an easy way to interface with limit switches connected to an Arduino or similar microcontroller. A limit switch is typically used to detect mechanical limits (such as position or travel limits) in automated systems.

### Features:

* Simple interface for reading the state of a limit switch.
* Supports easy initialization and querying of switch status.
* Designed for use with Arduino or compatible boards.
* Can be used for both open and closed-loop systems, detecting when a mechanical part reaches its limit.

---

## Circuit Setup

### Hardware Requirements:

* **Arduino or Compatible Microcontroller** (e.g., Arduino Uno, Mega, etc.)
* **Limit Switch**
* **External Pull-Down Resistor (1kΩ)**
* **Wires and Breadboard** (for prototyping)

### Wiring Diagram:

* **Limit Switch Pin**: One terminal of the limit switch connects to a digital input pin on the Arduino (e.g., pin 9).
* **GND**: The other terminal of the limit switch connects to the ground (GND).
* **Pull-Down Resistor**: A 1kΩ resistor is connected between the digital input pin (where the limit switch is connected) and GND. This ensures that the input pin reads LOW when the switch is not pressed, preventing floating pins.

### Circuit Diagram (Text-Based):

```
+5V -------------------+------------------- GND
                        |
                   [Limit Switch]
                        |
               Arduino Pin (e.g., Pin 9)
                        |
                 [1kΩ Pull-Down Resistor]
                        |
                      GND
```

### Explanation:

1. The limit switch has two terminals: one is connected to the Arduino pin (e.g., pin 9) and the other is connected to GND.
2. A 1kΩ pull-down resistor is connected between the Arduino input pin and GND to ensure the pin reads LOW when the switch is not triggered.
3. When the switch is pressed, the pin will read HIGH due to the connection to the +5V rail.
4. When the switch is released, the input pin will read LOW due to the pull-down resistor.

### Pin Connection:

* **Limit Switch**: Arduino Digital Pin 9 (example)
* **Pull-Down Resistor**: 1kΩ between Arduino Pin 9 and GND

---

## Installation

You can install the `LimitSwitch` library in your Arduino project by following these steps:

1. Download or clone the repository to your local machine.
2. Move the `LimitSwitch` folder to your Arduino libraries directory (`Documents/Arduino/libraries/`).
3. Restart your Arduino IDE (if it was open).
4. Now, you can include the library in your sketches by adding:

```cpp
#include <LimitSwitch.h>
```

---

## Usage Example

Here’s an example of how to use the `LimitSwitch` library:

```cpp
#include <LimitSwitch.h>

const byte limitSwitchPin = 9;  // Pin connected to the limit switch

// Create an instance of the LimitSwitch class
LimitSwitch limitSwitch(limitSwitchPin);

void setup() {
  Serial.begin(9600);
  
  // Initialize the limit switch (sets pin as input)
  limitSwitch.init();
}

void loop() {
  // Check if the limit switch is triggered
  if (limitSwitch.isTriggered()) {
    Serial.println("Limit switch triggered!");
  } else {
    Serial.println("Limit switch not triggered.");
  }

  // Wait a short time before checking again
  delay(500);
}
```

In this example, the limit switch is connected to pin 9 on the Arduino. The `init()` function initializes the pin, and `isTriggered()` checks whether the switch is pressed.

---

## Functions

### `LimitSwitch(byte pinNumber)`

* **Description**: Constructor to create a `LimitSwitch` object with a specified pin.
* **Parameters**: `pinNumber` (byte) - The Arduino pin number to which the limit switch is connected.

### `void init()`

* **Description**: Initializes the limit switch pin as an input.

### `bool isTriggered()`

* **Description**: Checks whether the limit switch is triggered.
* **Returns**: `true` if the limit switch is pressed (HIGH); `false` otherwise (LOW).

---

## Example Circuit

The circuit diagram below shows how the limit switch is connected to the Arduino:

```
Arduino Pin 9  ---[Limit Switch]--- GND
                             |
                       [1kΩ Pull-Down]
                             |
                           GND
```

---

## Notes

1. **External Pull-Down Resistor**: Using an external 1kΩ pull-down resistor is necessary to ensure that the input pin does not float and gives a stable LOW signal when the limit switch is not triggered.
2. **Digital Pin**: The library uses a digital pin for input, which works best with limit switches that produce a HIGH/LOW state.

---

## License

This library is released under the MIT License.

---

By using this library, you'll be able to easily integrate limit switches into your Arduino projects for tasks like detecting mechanical movement limits, position sensing, or triggering events when a physical boundary is reached.
