#include <Arduino.h>

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6
#define VIB_SENSOR_PIN 12
#define ANALOG_IN_MAX_VALUE 1023.0
#define POTENTIOMETER_IN A0

bool didVibrate = false;
size_t vibrationCount = 0;

void vibrated();

void setup() {
  // Add code here to set the pin modes for
  // the three pins connected to the LED
  // ??
  // ??
  // ??
  pinMode(VIB_SENSOR_PIN, INPUT_PULLUP);
  pinMode(POTENTIOMETER_IN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(VIB_SENSOR_PIN), vibrated, CHANGE);

  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');

    int red;
    int green;
    int blue;
    sscanf(line.c_str(), "%d,%d,%d", &red, &green, &blue);

    // In the code below, replace the "3", "5", and "6"
    // with something that makes the code easier to read.
    analogWrite(3, red);
    analogWrite(5, green);
    analogWrite(6, blue);
  }

  int potentiometerValue = analogRead(POTENTIOMETER_IN);
  int percentage = 100 * (potentiometerValue / ANALOG_IN_MAX_VALUE);
  Serial.print(didVibrate ? 1 : 0, DEC);
  Serial.print(",");
  Serial.println(percentage, DEC);

  if (didVibrate) {
    vibrationCount++;
  }
  
  didVibrate = false;
  delay(10);
}

void vibrated()
{
  didVibrate = true;
}