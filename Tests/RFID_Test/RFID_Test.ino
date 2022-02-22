//RFID Tag Unlocker
//RFID reader + screen + relay (not many IO, but can use D1,D0 and A5, should be OK if outputs. TX/RX leds are driven by 16u2, so should be ignored)
//relay on A5 so it doens't chatter. D0 for RFID RST, as it idles high anyway
//master user is first user and only has settings access (can't unlock)
//save settings to eeprom 8bytes card, 8 bytes pin, 14 bytes username, 2 byte permissions (allow access on card, allow access on pin) gives 32 users on Uno

#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
char hex[]="0123456789ABCDEF";

#define RST_PIN 0       
#define SS_PIN 10       
#define USERCOUNT 32
#define RELAYTIME 5000

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

int bx[] ={25,95,165,25,95,165,25,95,165,25,95,165};
int by[] ={10,10,10,70,70,70,130,130,130,190,190,190};
char bb[]="123456789#0*";
char kb[]="1234567890QWERTYUIOPASDFGHJKL'ZXCVBNM .<";   //soft keyboard
byte tstate[12]={0,0,0,0,0,0,0,0,0,0,0,0};
byte ltstate[12]={0,0,0,0,0,0,0,0,0,0,0,0};
char pin[10]="";
byte cardbytes[8];                                  //for reading routine
byte mcardbytes[8];                                 //for setting routine

void setup() {
  Serial.begin(9600);
  SPI.begin();              //start SPI
  mfrc522.PCD_Init();       //start RC522 module
  Serial.println("TAP CARD TO READER:");  
}

void loop() {
  donormal();
}

unsigned long getID(){
  unsigned long hex_num;
  hex_num =  mfrc522.uid.uidByte[0] << 24;
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] <<  8;
  hex_num += mfrc522.uid.uidByte[3];
  mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;
}

void donormal(){
  if(checkcard()){
    unsigned long uid = getID();
    if (uid == 29724) {
      Serial.println("Card Recognised!");
    }
    else if (uid == 1886) {
      Serial.println("Key Fob Recognised!");
    }
    else {
      Serial.println("Invalid Card!");
    }
  }
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
