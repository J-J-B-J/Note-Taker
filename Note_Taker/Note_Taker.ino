#include "Rotary.h"
#include "Text.h"

void setup() {
  // put your setup code here, to run once:
  //Open the serial port
  Serial.begin(9600);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
}

String oldRotaryLetters;

void loop() {
  String newRotaryLetters = rotary_encoder.getRotaryLetters("Both");
  if (oldRotaryLetters != newRotaryLetters){
    Serial.println(newRotaryLetters);
    oldRotaryLetters = newRotaryLetters;
  }
  
  note.updateButton();
}
