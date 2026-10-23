#include <Arduino.h>

const int greenNumber = 0;  // int representing the green LED
const int redNumber = 1;    // int representing the red LED
const int yellowNumber = 2; // int representing the yellow LED
const int blueNumber = 3;   // int representing the blue LED

const int listOfRandomNumbersSize = 20;           // int representing the size of the array to store random numbers (max length of game)
const int rangeRandom = 4;                        // int representing the range of random numbers (0 to 3)
int randNumber;                                   // int representing the current random number
int listOfRandomNumbers[listOfRandomNumbersSize]; // array to store the sequence of random numbers
int temp;                                         // temporary variable for storing intermediate values

// Array index matches each LED's assigned number:
// 0 = green, 1 = red, 2 = yellow, 3 = blue
const int ledPins[rangeRandom] = {4, 5, 6, 7};

const int ledOnTime = 500;  // LED stays on for 500 milliseconds
const int ledOffTime = 250; // Pause between flashes

void setup()
{                                                         // put your setup code here, to run once:
  Serial.begin(9600);                                     // initialize serial communication
  int seed = analogRead(A0);                              // Read an analog value from pin A0 to use as the random seed
  Serial.println("Beginning with seed: " + String(seed)); // print the seed value
  randomSeed(seed);                                       // initialize random seed
  for (int i = 0; i < rangeRandom; i++)                   // Loop through each LED pin and initialize
  {
    pinMode(ledPins[i], OUTPUT);   // Set each LED pin as an output
    digitalWrite(ledPins[i], LOW); // Ensure each LED is initially off
  }
}

void loop()
{
  randNumber = random(rangeRandom); // Generate a random color number: 0, 1, 2, or 3.
  Serial.println(randNumber);       // Print the generated random number to the serial monitor
  digitalWrite(ledPins[randNumber], HIGH); // Turn on the LED corresponding to the random number
  delay(ledOnTime);                        // Wait for the specified on time before turning off the LED
  digitalWrite(ledPins[randNumber], LOW); // Turn off the LED corresponding to the random number
  delay(ledOffTime);                      // Wait for the specified off time before the next loop iteration
}