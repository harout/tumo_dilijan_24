#include <Arduino.h>


void setup() {
  Serial.begin(115200);

  // This flashes out the pattern 01010101....
  for (int i = 0; i < 100; i++) 
  {  
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
}

void loop() {
  // This flashes out the pattern 011011011....

  
}

