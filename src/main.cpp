#include <Arduino.h>

const int greenNumber = 0; // int representing the green LED
const int redNumber = 1; // int representing the red LED
const int yellowNumber = 2; // int representing the yellow LED
const int blueNumber = 3; // int representing the blue LED
const int listOfRandomNumbersSize = 20; // int representing the size of the array to store random numbers (max length of game)
const int rangeRandom = 4; // int representing the range of random numbers
int randNumber; // int representing the current random number
int listOfRandomNumbers[listOfRandomNumbersSize]; // array to store the sequence of random numbers
int temp; // temporary variable for storing intermediate values

void setup() { // put your setup code here, to run once:
  Serial.begin(9600); // initialize serial communication
  int seed = analogRead(A0);
  Serial.println("Beginning with seed: " + String(seed)); // print the seed value
  randomSeed(seed); // initialize random seed
}

void loop() { // put your main code here, to run repeatedly:
  randNumber = random(rangeRandom); // generate a random number between 0 and 299
    for (int i = 0; i < listOfRandomNumbersSize; i++) {
      temp = random(rangeRandom);
      listOfRandomNumbers[i] = random(rangeRandom);
    }
  Serial.println(randNumber); // print the random number
  delay(100); // wait for 1 second
}