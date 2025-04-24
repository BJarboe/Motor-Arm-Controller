#include <ESP32Servo.h>

const int NUM_JOINTS = 3;
const int SMOOTHING_WINDOW = 10;

const int potPins[NUM_JOINTS] = {27, 32, 33};
const int servoPins[NUM_JOINTS] = {25, 26, 14};

const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;
const int MIN_ADC = 0;
const int MAX_ADC = 4095;

Servo servos[NUM_JOINTS];

// Smoothing buffers and indices
int readings[NUM_JOINTS][SMOOTHING_WINDOW] = {0};
int readIndex[NUM_JOINTS] = {0};
int total[NUM_JOINTS] = {0};

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");
  
  /*attenuation determines range of input voltages the ADC can accurately measure
    ADC = Analog-Digital conversion
    ADC_11db enumerator defining 11 dB attenuation => up to 3.9V   */
  analogSetAttenuation(ADC_11db);

  for (int i = 0; i < NUM_JOINTS; i++) {
    servos[i].setPeriodHertz(50);
    servos[i].attach(servoPins[i]);
    servos[i].write(0);

    // Initialize smoothing buffers
    for (int j = 0; j < SMOOTHING_WINDOW; j++) {
      readings[i][j] = analogRead(potPins[i]);
      total[i] += readings[i][j];
    }
  }

  Serial.println("Setup complete.");
}

void loop() {
  for (int i = 0; i < NUM_JOINTS; i++) {
    int raw = analogRead(potPins[i]);

    // Remove oldest reading
    total[i] -= readings[i][readIndex[i]];
    // Replace with new reading
    readings[i][readIndex[i]] = raw;
    total[i] += raw;

    readIndex[i] = (readIndex[i] + 1) % SMOOTHING_WINDOW;

    float avg = total[i] / (float)SMOOTHING_WINDOW;
    float angle = analogToAngle(avg, MIN_ADC, MAX_ADC, MIN_ANGLE, MAX_ANGLE);

    Serial.print("Joint ");
    Serial.print(i);
    Serial.print(" - Smoothed: ");
    Serial.print(avg);
    Serial.print(" -> Angle: ");
    Serial.println(angle);

    servos[i].write(angle);
  }

  delay(40);
}
// Convert voltage ratio to angle using proportional mapping
float analogToAngle(float value, float min_reading, float max_reading, float min_angle, float max_angle) {
  return (value - min_reading) / (max_reading - min_reading) * (max_angle - min_angle) + min_angle;
}
