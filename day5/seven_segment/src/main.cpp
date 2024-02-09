#include <Arduino.h>

#define ANALOG_IN_MAX_VALUE 1023.0

#define POTENTIOMETER_IN A0
#define LATCH_PIN 3
#define CLOCK_PIN 4
#define DATA_PIN 2
#define VIB_SENSOR_PIN 12

#define E_SEG  0b00000001
// define the other segments
// ?????
// ?????
// ?????
// ...

#define ZERO  (A_SEG | B_SEG | C_SEG | D_SEG | E_SEG | F_SEG)
// define the other numbers
// ????
// ????
// ????
// ...

uint8_t digits[10] = {ZERO, ONE, ???};
uint8_t segments[7] = {A_SEG, ???};
bool didVibrate = false;
size_t vibrationCount = 0;

void vibrated();

void setup() {
  attachInterrupt(digitalPinToInterrupt(VIB_SENSOR_PIN), vibrated, CHANGE);

  // we need to configure the LATCH_PIN, CLOCK_PIN, DATA_PIN, and POTENTIOMETER_IN
  // ????
  // ????
  // ????

  Serial.begin(9600);
}


void loop() {
  // Write code to count from 0 - 9

  // write code to animate the segments in some interesting manner

  // Analog read of potentiometer value
  int potentiometerValue = ...
  int scaled = ANALOG_OUT_MAX_VALUE * (potentiometerValue / ANALOG_IN_MAX_VALUE);
  
  if (didVibrate) {
    vibrationCount++;
  }
  
  didVibrate = false;
  Serial.print(scaled, DEC);
  Serial.print(",");
  Serial.println(vibrationCount, DEC);

  delay(10);
}

void vibrated()
{
  didVibrate = true;
}