class Rotary {
  private:
    int checkrotary(int rotary1, int rotary2){
    static byte old=0;
    byte new1 = digitalRead(rotary1);
    byte new2 = digitalRead(rotary2);
    byte new_var = (new1*2)|(new2^new1);
    byte d = old-new_var;
    old=new_var;
    switch(d){
      case 1: return 1;
      case 3: return -1;
      case 255: return -1;
      case 253: return 1;
      default: return 0;
      }
    }
  public:
    int s=0;

    void printRotary() {
      while (true){
        int d = checkrotary(6, 7);
        s=s+d;
        int t=(s/4);
  
        if(d){Serial.println(t);}
      }
    }
};

void setup() {
  // put your setup code here, to run once:
  //Open the serial port
  Serial.begin(9600);
  Rotary rotary_encoder;
  rotary_encoder.printRotary();
  
}

void loop() {

}

int checkrotary(int rotary1, int rotary2){
  static byte old=0;
  byte new1 = digitalRead(rotary1);
  byte new2 = digitalRead(rotary2);
  byte new_var = (new1*2)|(new2^new1);
  byte d = old-new_var;
  old=new_var;
  switch(d){
    case 1: return 1;
    case 3: return -1;
    case 255: return -1;
    case 253: return 1;
    default: return 0;
  }
}
