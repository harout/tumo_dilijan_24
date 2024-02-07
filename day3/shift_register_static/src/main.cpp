#include <Arduino.h>
#include <SPI.h>

#define LATCH_PIN 3 // connect to rclk
#define CLOCK_PIN 4 // connects to srclk
#define DATA_PIN 2 // connects to ser

void setup() {
  Serial.begin(9600);

  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT); 
}

void loop() {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b10101010);
  digitalWrite(LATCH_PIN, HIGH);
}