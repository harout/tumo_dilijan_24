#include <Arduino.h>

#define LATCH_PIN 3
#define CLOCK_PIN 4
#define DATA_PIN 2

void setup() {
  Serial.begin(9600);

  // The LATCH_PIN, CLOCK_PIN, and DATA_PIN
  // are going to be used as outputs. Add code
  // here to set the mode of these pins to OUTPUT
  // ???
  // ???
  // ???
}

void loop() {
  if (Serial.available() > 0) {
    // Add code here to read a byte of data from
    // Serial. This byte is used to contorl which 
    // LEDs are illuminated.
    // ???

    // The LATCH_PIN needs to be set to LOW before we 
    // start sending the shift register the data. Add
    // code here to set the LATCH_PIN to low.
    // ????

    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, incomingByte);
    
    // The LATCH_PIN needs to be set to HIGH after we 
    // finish sending the shift register the data. Add
    // code here to set the LATCH_PIN to high.
    // ????
  }
}