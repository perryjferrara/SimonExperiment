#include <Arduino.h>

void setup() { // put your setup code here, to run once:
  Serial.begin(9600); // initialize serial communication
  int seed = analogRead(A0);
  Serial.println("Beginning with seed: " + String(seed)); // print the seed value
  randomSeed(seed); // initialize random seed
}

void loop() { // put your main code here, to run repeatedly:
  int randNumber = random(300); // generate a random number between 0 and 299
  Serial.println(randNumber); // print the random number
  delay(100); // wait for 1 second
}
