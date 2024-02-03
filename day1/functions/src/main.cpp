#include <Arduino.h>

void initArray(int[], int);
void doubleArray(int[], int);
void printArray(int[], int);
void printArrayP(int*, int);

void setup() {
  Serial.begin(115200);

  Serial.println("Hello World!");

  int numbers[10];
  initArray(numbers, 10);

  Serial.println("__Numbers__");
  printArray(numbers, 10);
  Serial.println();
  Serial.println();

  doubleArray(numbers, 10);
  Serial.println("__Doubled Numbers__");
  printArray(&numbers[0], 10);
  Serial.println();
  Serial.println();

  doubleArray(numbers, 10);
  Serial.println("__Quadrupled Numbers__");
  printArrayP(numbers, 10);
  Serial.println();
  Serial.println();

  // Reset the array to hold the numbers 0...9
  // ????

  // Use a function call to multiply all the numbers by 3
  // ????

  // Print the list of numbers
  // ????

  // Use a function call to multiply all the numbers by 5
  // ????

  // Print the list of numbers
  // ????  
}

void loop() {
  
}


void initArray(int array[], int size) {
  for (int i = 0; i < size; i++) {
    array[i] = 0;
  }
}

void doubleArray(int array[], int size) {
  for (int i = 0; i < size; i++) {
    array[i] *= 2;
  }
}

void printArray(int array[], int size) {
  for (int i = 0; i < size; i++) {
    Serial.println(array[i]);
  }
}


void printArrayP(int* a, int size) {
  for (int i = 0; i < size; i++) {
    Serial.println(*a);
    a += 1;
  }
}