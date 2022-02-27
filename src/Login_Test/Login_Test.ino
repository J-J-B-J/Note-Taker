#include "Settings.h"
#include "Login_Functions.h"

void setup() {
  Serial.begin(74480);
  Unlock_With_RFID();
}

void loop() {}
