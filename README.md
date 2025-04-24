# ESP32 Motor Arm Controller

This project uses an ESP32 to control a 3-joint robotic arm. Each joint has a servo motor controlled by a corresponding rotary potentiometer. Analog readings from the potentiometers are smoothed using a moving average before being mapped to servo angles. The result is stable and responsive control of each arm segment.

## Features

- 3-joint control using three potentiometers and three servo motors
- Smoothing of analog input using a moving average (sliding window)
- Angle mapping from ADC readings (0–4095) to standard servo angles (0–180 degrees)
- Basic real-time loop for consistent input-to-output updates

## Hardware Requirements

- ESP32 microcontroller
- 3 servo motors (MG90s)
- 3 rotary potentiometers (10kΩ)
- Breadboard and jumper wires
- External 5V power supply for the servos

## Pin Configuration

| Joint | Potentiometer Pin | Servo Pin |
|-------|-------------------|-----------|
| 1     | GPIO 27           | GPIO 25   |
| 2     | GPIO 33           | GPIO 26   |
| 3     | GPIO 32           | GPIO 14   |

All analog pins should be capable of ADC input on the ESP32.

## How It Works

1. Each potentiometer feeds a voltage signal into an ADC pin on the ESP32.
2. A small circular buffer stores the last N readings per joint.
3. The moving average of the buffer smooths out noise and sudden fluctuations.
4. The smoothed value is mapped to an angle between 0 and 180 degrees.
5. The angle is sent to the corresponding servo motor.

## Notes

- Potentiometers should be wired with one side to GND, the other to 3.3V, and the center (wiper) to the analog input.
- Do not power servos directly from the ESP32’s onboard voltage pins. Use a dedicated 5V power source, and connect the grounds together.
- The smoothing window size can be adjusted for responsiveness vs. stability. A larger window smooths better but responds more slowly.
- There is no clamping or fault handling by default. It is recommended to add safeguards before using this in a critical application.
