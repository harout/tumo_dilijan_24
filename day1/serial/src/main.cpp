#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(115200);
}

void loop() {
  String name = "";
  
  while(Serial.available())
  {
    Serial.read();
  }

  // Read a character from the serial monitor
  // and append it to the name string. Stop when
  // the user presses enter.
  Serial.print("Hello, what is your name? ");
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '\n') {
        break;
      }
      name += c;
      Serial.print(c);
    }
  }
  Serial.println();
  Serial.println("Hello, " + name);
  

  bool pickedValidNumber = false;
  while (!pickedValidNumber)
  {
    Serial.print("Pick a number 1 through 10? ");
    String numStr = "";
    while (true) {
      if (Serial.available() > 0) {
        char c = Serial.read();
        if (c == '\n') {
          break;
        }
        numStr += c;
        Serial.print(c);
      }
    }

    Serial.println();
    int number = numStr.toInt();
    if (number < 1 || number > 10) {
      Serial.println("You picked an invalid number");
    } else {
      Serial.println("You picked " + String(number));
      pickedValidNumber = true;
    }
    Serial.println();
  }
  Serial.println();


  // Let's pick a secret number
  int secretNumber = random(1, 11);
  
  // Ask the user to guess the secret number.
  // Let the user know if the guess was too high, too low, or correct.
}
