#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
char hex[]="0123456789ABCDEF";

#define RST_PIN 0
#define SS_PIN 10
#define USERCOUNT 32

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

char pin[10]="";
byte cardbytes[8];                                  //for reading routine
byte mcardbytes[8];                                 //for setting routine

void setup() {
  SPI.begin();              //start SPI
  mfrc522.PCD_Init();       //start RC522 module
  pinMode(0,INPUT);         //use serial pullup to hold high
  int e=0;
  for(int i=0;i<16;i++){e=e+EEPROM.read(i);}       //check EEPROM contents (255=blank)
  if(e==16*255){dosetup();}    //first time setup if no master user set
  Serial.println("SWIPE CARD:");  
}

void loop() {
  char a;
  int s;
  s=strlen(pin);
  if(checkcard()){
    Serial.println("CARD");
    docard();                                   //process card
    delay(100);
  }else{
    Serial.println("CARD");
  }
}


void dounlock(int u){             //unlock and display welcome message
  char uname[16]="";
  for(int i=0;i<14;i++){uname[i]=EEPROM.read(u*32+16+i);}
  uname[14]=0;
  Serial.println("Unlocked!");
  Serial.println("SWIPE CARD:");  
}

void docard(){
  int umatch=-1;                //user to be matched
  byte match;                   //flag for mismatch
  match=1;
  for(int n=0;n<8;n++){
    if(EEPROM.read(1*32+n)!=cardbytes[n]){match=0;}   //mismatch found, clear
  }
  if(EEPROM.read(1*32+30)==0){match=0;}           //pin not allowed for this user
  if(match){umatch=1;}                            //flag matched user
  if(umatch>0){  
    dounlock(umatch);                               //do unlock routine for user
  }else{
    Serial.println("CARD ERROR");      //error message
    delay(1000);  
    Serial.println("CARD ERROR");
  }
}


void editcard(int u){   //swipe new card- need to check if it matches an existing one before validating
  byte cardset=0;
  byte match;                   //flag for mismatch
  int umatch=-1;
  cardset=getcard();                                                              //get a card, returns 0 if no card selected
  for(int i=0;i<USERCOUNT;i++){
    match=1;
    for(int n=0;n<8;n++){
      if(EEPROM.read(i*32+n)!=mcardbytes[n]){match=0;}   //mismatch found, clear
    }
    if(match){umatch=i;}                            //flag matched user
  }
  if(umatch>=0){cardset=0;Serial.println("CARD ALREADY IN USE!");}  //card already used
  if(cardset){for(int i=0;i<8;i++){EEPROM.write(i+u*32, mcardbytes[i]);}}              //copy to EEPROM
}

void editpin(int u){    //enter new PIN- need to check if it matches an existing one before validating
  byte pinset=0;
  byte match;                   //flag for mismatch
  int umatch=-1;
    for(int i=0;i<USERCOUNT;i++){
    match=1;
    for(int n=0;n<8;n++){
      if(EEPROM.read(i*32+n+8)!=pin[n]){match=0;}   //mismatch found, clear
    }
    if(match){umatch=i;}                            //flag matched user
  }
  if(umatch>=0){pinset=0;Serial.println("PIN ALREADY IN USE!");}  //PIN already used
  if(pinset){for(int i=0;i<8;i++){EEPROM.write(i+8+u*32, pin[i]);}}           //copy to EEPROM
  for(int i=0;i<8;i++){pin[i]=0;}                                             // clear array for main program
}

void drawuserinfo(int u){
  int e;
  char uname[16]="";
  for(int i=0;i<14;i++){uname[i]=EEPROM.read(u*32+16+i);}
  uname[14]=0;
  Serial.println("USER INFO:");
  Serial.println((u/10)%10+'0');
  Serial.println((u)%10+'0');
  Serial.println("Name:");
  Serial.println(uname);
  e=0;
  for(int i=0;i<8;i++){e=e+EEPROM.read(u*32+i);}      //check if card set or all 0xFF
  Serial.println("Card:");
  if(e==8*0xFF){Serial.println("NOT SET");}else{Serial.println("SET");}
  if(EEPROM.read(u*32+30)){Serial.println("ACTIVE");}else{Serial.println("DISABLED");}
  e=0;
  for(int i=0;i<8;i++){e=e+EEPROM.read(u*32+i+8);}      //check if pin set or all 0xFF
  Serial.println("PIN:");
  if(e==8*0xFF){Serial.println("NOT SET");}else{Serial.println("SET");}
  if(EEPROM.read(u*32+31)){Serial.println("ACTIVE");}else{Serial.println("DISABLED");}
}

void dosetup(){
  byte cardset=0;
  byte pinset=0;
  Serial.println("MASTER USER SETUP");                         //warning for master setup
  cardset=getcard();                                                              //get a card, returns 0 if no card selected
  Serial.println("MASTER USER SETUP");                         //warning for master setup
  if(cardset){for(int i=0;i<8;i++){EEPROM.write(i, mcardbytes[i]);}}              //copy to EEPROM
  if(pinset){for(int i=0;i<8;i++){EEPROM.write(i+8, pin[i]);pin[i]=0;}}           //copy to EEPROM, clear array for main program
  EEPROM.write(30,cardset);         //write card permission
  EEPROM.write(31,pinset);          //write card permission  
}

byte getcard(){       //get a swiped card for setup
  byte done=0;
  byte cardset=0;
  for(int i=0;i<8;i++){mcardbytes[i]=0;}
  Serial.println("Swipe a card to set");
  while(!done){
    if(checkcard()){          //valid card swiped
      for(int i=0;i<8;i++){
        mcardbytes[i]=cardbytes[i];
        Serial.println(hex[mcardbytes[i]>>4]);
        Serial.println(hex[mcardbytes[i]&15]);
      }
      cardset=1;
    }
    done=1;                                  //keep card value  
  }
  return cardset;
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
