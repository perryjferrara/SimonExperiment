#include <Arduino.h>
#include "pitches.h"

#define BUZZ_PASSIVE_PIN 8 // Pin connected to the passive buzzer

const int greenNumber = 0;      // int representing the green LED
const int redNumber = 1;        // int representing the red LED
const int yellowNumber = 2;     // int representing the yellow LED
const int blueNumber = 3;       // int representing the blue LED
const int correctLEDPin = 9;    // Pin connected to the LED indicating a correct sequence
const int incorrectLEDPin = 10; // Pin connected to the LED indicating an incorrect sequence

const int listOfRandomNumbersSize = 20;           // int representing the size of the array to store random numbers (max length of game)
const int rangeRandom = 4;                        // int representing the range of random numbers (0 to 3)
int randNumber;                                   // int representing the current random number
int listOfRandomNumbers[listOfRandomNumbersSize]; // array to store the sequence of random numbers
int temp;                                         // temporary variable for storing intermediate values

const int ledPins[rangeRandom] = {4, 5, 6, 7};                          // Array storing the LED pins for each color
const int ledNotes[rangeRandom] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5}; // Array storing the notes corresponding to each LED color

const int ledOnTime = 500;           // LED stays on for 500 milliseconds
const int ledOffTime = 250;          // Pause between flashes
const int buzzerOnTime = 150;        // Duration to keep the buzzer on for correct sequence
const int buzzerIncorrectTime = 300; // Duration to keep the buzzer on for incorrect sequence

void correctSequence()
{
  digitalWrite(correctLEDPin, HIGH);  // Turn on the correct sequence LED
  digitalWrite(incorrectLEDPin, LOW); // Turn off the incorrect sequence LED

  tone(BUZZ_PASSIVE_PIN, NOTE_C5); // Play first tone on the passive buzzer
  delay(buzzerOnTime);             // Keep the tone on for the specified time

  tone(BUZZ_PASSIVE_PIN, NOTE_E5); // Play another tone on the passive buzzer
  delay(buzzerOnTime);             // Wait for the specified buzzer on time before continuing

  tone(BUZZ_PASSIVE_PIN, NOTE_G5); // Play highest pitch tone on the passive buzzer
  delay(buzzerOnTime * 2);         // Wait for the specified buzzer on time
  noTone(BUZZ_PASSIVE_PIN);        // Stop the sound on the passive buzzer

  digitalWrite(correctLEDPin, LOW); // Turn off the correct sequence LED
}

void incorrectSequence()
{
  digitalWrite(correctLEDPin, LOW);    // Turn off the correct sequence LED
  digitalWrite(incorrectLEDPin, HIGH); // Turn on the incorrect sequence LED

  tone(BUZZ_PASSIVE_PIN, NOTE_G3); // Play a tone on the passive buzzer
  delay(buzzerIncorrectTime * 2);  // Keep the tone on for the specified time

  digitalWrite(incorrectLEDPin, LOW); // Turn off the incorrect sequence LED before playing the next tone
  noTone(BUZZ_PASSIVE_PIN);           // Stop the sound on the passive buzzer before the next tone
  delay(buzzerIncorrectTime);         // Wait for the specified buzzer on time before continuing

  digitalWrite(incorrectLEDPin, HIGH); // Turn on the incorrect sequence LED before stopping the tone
  tone(BUZZ_PASSIVE_PIN, NOTE_C3);     // Play another tone on the passive buzzer
  delay(buzzerIncorrectTime * 2);      // Keep the tone on for the specified time before stopping it

  noTone(BUZZ_PASSIVE_PIN); // Stop the sound on the passive buzzer

  digitalWrite(incorrectLEDPin, LOW); // Turn off the incorrect sequence LED
}

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
  pinMode(correctLEDPin, OUTPUT);     // Set the correct LED pin as an output
  pinMode(incorrectLEDPin, OUTPUT);   // Set the incorrect LED pin as an output
  digitalWrite(correctLEDPin, LOW);   // Ensure the correct LED is initially off
  digitalWrite(incorrectLEDPin, LOW); // Ensure the incorrect LED is initially off

  pinMode(BUZZ_PASSIVE_PIN, OUTPUT); // Set the passive buzzer pin as an output
}

void loop()
{
  // randNumber = random(rangeRandom);             // Generate a random color number: 0, 1, 2, or 3.
  // Serial.println(randNumber);                   // Print the generated random number to the serial monitor
  // digitalWrite(ledPins[randNumber], HIGH);      // Turn on the LED corresponding to the random number
  // tone(BUZZ_PASSIVE_PIN, ledNotes[randNumber]); // Play the corresponding note on the passive buzzer
  // delay(ledOnTime);                             // Wait for the specified on time before turning off the LED
  // digitalWrite(ledPins[randNumber], LOW);       // Turn off the LED corresponding to the random number
  // noTone(BUZZ_PASSIVE_PIN);                     // Stop the sound on the passive buzzer
  // delay(ledOffTime);                            // Wait for the specified off time before the next loop iteration

  // Testing the buzzer and LEDs for correct sequence
  Serial.println("Testing correct round feedback");
  correctSequence(); // Call the function to test the correct sequence
  delay(1000);       // Wait for a second before testing the next sequence

  Serial.println("Testing incorrect round feedback");
  incorrectSequence(); // Call the function to test the incorrect sequence
  delay(1000);         // Wait for a second before the next loop iteration
}