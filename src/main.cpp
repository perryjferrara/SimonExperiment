#include <Arduino.h>
#include "pitches.h"

#define BUZZ_PASSIVE_PIN 8 // Pin connected to the passive buzzer

const int greenNumber = 0;      // int representing the green LED
const int redNumber = 1;        // int representing the red LED
const int yellowNumber = 2;     // int representing the yellow LED
const int blueNumber = 3;       // int representing the blue LED
const int correctLEDPin = 9;    // Pin connected to the LED indicating a correct sequence
const int incorrectLEDPin = 10; // Pin connected to the LED indicating an incorrect sequence

const int listOfRandomNumbersSize = 20;            // int representing the size of the array to store random numbers (max length of game)
const int rangeRandom = 4;                        // int representing the range of random numbers (0 to 3)
int randNumber;                                   // int representing the current random number
int listOfRandomNumbers[listOfRandomNumbersSize]; // array to store the sequence of random numbers
int temp;                                         // temporary variable for storing intermediate values

const int ledPins[rangeRandom] = {4, 5, 6, 7};                          // Array storing the LED pins for each color
const int ledNotes[rangeRandom] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5}; // Array storing the notes corresponding to each LED color
const int buttonPins[rangeRandom] = {2, 3, 11, 12};                     // Array storing the button pins for each color

int sequenceLength = 1;      // int representing the current length of the sequence
const int debounceTime = 25; // Debounce time for button presses in milliseconds

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

void playColor(int colorNumber) // Function to play the LED and corresponding sound for a given color number
{
  digitalWrite(ledPins[colorNumber], HIGH);      // Turn on the LED corresponding to the color number
  tone(BUZZ_PASSIVE_PIN, ledNotes[colorNumber]); // Play the corresponding note on the passive buzzer
  delay(ledOnTime);                              // Wait for the specified on time before turning off the LED
  digitalWrite(ledPins[colorNumber], LOW);       // Turn off the LED corresponding to the color number
  noTone(BUZZ_PASSIVE_PIN);                      // Stop the sound on the passive buzzer
  delay(ledOffTime);                             // Wait for the specified off time before continuing
}

void waitForButtonRelease() // Function to wait for a button press with debounce
{
  bool allButtonsReleased;            // Variable to track if the button has been released
  unsigned long startTime = millis(); // Record the start time for the debounce check

  while (millis() - startTime < debounceTime)
  {
    allButtonsReleased = true; // Assume all buttons are released initially
    for (int i = 0; i < rangeRandom; i++)
    {
      if (digitalRead(buttonPins[i]) == LOW) // Check if any button is still pressed
      {
        allButtonsReleased = false; // If a button is pressed, set the flag to false
      }
    }

    if (!allButtonsReleased) // If any button is still pressed, reset the debounce timer
    {
      startTime = millis(); // Reset the start time if any button is still pressed
    }
  }
}

int readButton() // Function to read the state of a button with debounce
{
  while (true) // Loop indefinitely until a button press is detected
  {
    for (int i = 0; i < rangeRandom; i++) // Loop through each button pin to check if it is pressed
    {
      if (digitalRead(buttonPins[i]) == LOW) // Check if the button is pressed
      {
        delay(debounceTime);                   // Wait for the debounce time before checking again
        if (digitalRead(buttonPins[i]) == LOW) // Confirm the button is still pressed after debounce time
        {
          return i; // Return the index of the button that was pressed
        }
      }
    }
  }
}

void setup()
{                                                         // put your setup code here, to run once:
  Serial.begin(9600);                                     // initialize serial communication
  int seed = analogRead(A0);                              // Read an analog value from pin A0 to use as the random seed
  Serial.println("Beginning with seed: " + String(seed)); // print the seed value
  randomSeed(seed);                                       // initialize random seed
  for (int i = 0; i < rangeRandom; i++)                   // Loop through each LED pin and initialize
  {
    pinMode(ledPins[i], OUTPUT);          // Set each LED pin as an output
    digitalWrite(ledPins[i], LOW);        // Ensure each LED is initially off
    pinMode(buttonPins[i], INPUT_PULLUP); // Set each button pin as an input with internal pull-up resistor
  }
  pinMode(correctLEDPin, OUTPUT);     // Set the correct LED pin as an output
  pinMode(incorrectLEDPin, OUTPUT);   // Set the incorrect LED pin as an output
  digitalWrite(correctLEDPin, LOW);   // Ensure the correct LED is initially off
  digitalWrite(incorrectLEDPin, LOW); // Ensure the incorrect LED is initially off

  pinMode(BUZZ_PASSIVE_PIN, OUTPUT); // Set the passive buzzer pin as an output
}

void loop()
{
  waitForButtonRelease(); // Wait for the user to press a button before continuing the loop
  delay(1000);            // Small delay to debounce the button release

  listOfRandomNumbers[sequenceLength - 1] = random(rangeRandom); // Call the random function to generate a new number for the sequence

  Serial.println("Sequence Length: " + String(sequenceLength) + " | New random number added to sequence: " + String(listOfRandomNumbers[sequenceLength - 1])); // Print the updated sequence information
  Serial.print("Pay attention to the sequence!");

  for (int i = 0; i < sequenceLength; i++) // Loop through the sequence and display each LED
  {
    playColor(listOfRandomNumbers[i]); // Play the color corresponding to the current sequence element
  }

  waitForButtonRelease();                            // Wait for the user to release the button before continuing
  Serial.println();                                  // Print a blank line for better readability
  Serial.println("Your turn! Repeat the sequence."); // Prompt the user to repeat the sequence

  for (int i = 0; i < sequenceLength; i++) // Loop through the sequence and get user input for each LED
  {
    int userInput = readButton();                    // Wait for the user to press a button and get the corresponding LED index
    Serial.println("Pressed: " + String(userInput)); // Print the user input for debugging purposes

    playColor(userInput);                    // Play the color corresponding to the user's input
    waitForButtonRelease();                  // Wait for the user to release the button before continuing
    if (userInput != listOfRandomNumbers[i]) // Check if the user input matches the sequence
    {
      Serial.println("Incorrect input! Expected: " + String(listOfRandomNumbers[i])); // Print the expected input for debugging purposes
      incorrectSequence();                                                            // Call the function to indicate an incorrect sequence
      sequenceLength = 1;                                                             // Reset the sequence length to start over
      return;                                                                         // Exit the loop and start over
    }
  }

  if (sequenceLength == listOfRandomNumbersSize) // Check if the user has successfully completed the entire sequence
  {
    Serial.println("Correct sequence! You completed all " + String(listOfRandomNumbersSize) + " steps."); // Print a message indicating the correct sequence for debugging purposes
    correctSequence();                                                                                    // Call the function to indicate a correct sequence
    sequenceLength = 1;                                                                                   // Increase the sequence length for the next round
  }
  else
  {
    sequenceLength++; // Increase the sequence length for the next round
  }
}