#include "Rotary.h"

Rotary rotary_encoder;

void setup() {
  Serial.begin(9600);
  rotary_encoder.begin();
}

void loop() {
  rotary_encoder.getTypedLetters();
}
