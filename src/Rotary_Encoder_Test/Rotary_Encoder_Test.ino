#include "Settings.h"
#include "Rotary.h"

Rotary rotary_encoder;

void setup() {
  Serial.begin(74880);
  rotary_encoder.begin();
}

void loop() {
  rotary_encoder.getTypedLetters();
}
