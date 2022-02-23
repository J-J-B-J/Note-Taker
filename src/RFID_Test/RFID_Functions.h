#include <SPI.h>
#include <MFRC522.h>

#define Valid_RFIDs {29724, 1886;}

#define RST_PIN 0
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

byte cardbytes[8];                                  //for reading routine

unsigned long getID(){
  unsigned long hex_num;
  hex_num =  mfrc522.uid.uidByte[0] << 24;
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] <<  8;
  hex_num += mfrc522.uid.uidByte[3];
  mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;
}

int checkcard(){
  for(byte i=0;i<8;i++){cardbytes[i]=0;}
  if (!mfrc522.PICC_IsNewCardPresent()){return 0;}      //no card found
  if (!mfrc522.PICC_ReadCardSerial()){return 0;}        //no card found
  for(byte i = 0; i < mfrc522.uid.size; i++){
    if(i<8){cardbytes[i]=mfrc522.uid.uidByte[i];}
  }
  mfrc522.PICC_HaltA();
  return 1;                                             //card ID read
}

void Unlock_With_RFID(){
  SPI.begin();
  mfrc522.PCD_Init();       //start RC522 module
  Serial.println("TAP CARD TO READER:");
  do {} while(!mfrc522.PICC_IsNewCardPresent());
  unsigned long uid = getID();
  bool Success = false;
  for (int i; i < sizeof(Valid_RFIDs); i++) {
    if (uid == i) {
      Success = true;
      break;
    }
  }
  if (Success) {
    Serial.println("You Did It!");
  }
  else {
    Serial.println("Entry Failed!");
  }
}
