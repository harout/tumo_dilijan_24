#include <Arduino.h>

int evens[] = {0, 2, 4, 6, 8}

void setup() {
  Serial.begin(115200);

  Serial.println("Here are the first five even numbers:");
  for (int i = 0; i < 5; i++) {
    Serial.println(evens[i]);
  }
  Serial.println();

  Serial.println("Here are the first 8 prime numbers:");
  // ???

  Serial.println("Here are the first 8 prime numbers in reverse:");
  // ???
}

void loop() {
  
}
