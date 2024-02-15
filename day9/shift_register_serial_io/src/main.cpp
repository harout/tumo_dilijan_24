#include <Arduino.h>

#define LATCH_PIN 3
#define CLOCK_PIN 4
#define DATA_PIN 2

void setup() {
  Serial.begin(9600);

  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();

    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, incomingByte);
    digitalWrite(LATCH_PIN, HIGH);
  }
}