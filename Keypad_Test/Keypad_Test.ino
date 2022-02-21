#define Passcode String(25364781)

byte Key;
byte OldKey = 0;

void setup(){
  Serial.begin(9600);
  
  Unlock_With_Keypad();
}


void loop() {
}


void Unlock_With_Keypad() {
  bool Success = false;
  for (int i = 0; i<4; i++) {
    unsigned long Start_Time = millis();
    String Code = Get_Code();
    unsigned long End_Time = millis();
    unsigned long Time_Taken = End_Time - Start_Time;
    if (Time_Taken > 5000) {
      Serial.println("You took too long!");
    }
    else if (Code != Passcode) {
      Serial.println("Incorrect Passcode!");
    }
    else {
      Success = true;
      break;
    }
  }
  if (Success == true) {
    Serial.println("You Did It!");
  }
  else {
    Serial.println("Entry Failed!");
  }
}

String Get_Code() {
  Serial.println("Enter Code:");

  String Code = "";

  do {
    for (int pin = 2; pin < 10; pin++) {
      if (digitalRead(pin) == HIGH) {
        Code += String(pin-1);
        Serial.println(Code);
        do {} while (digitalRead(pin) == HIGH);
      }
    }
  } while (Code.length() < 8);

  return Code;
}
