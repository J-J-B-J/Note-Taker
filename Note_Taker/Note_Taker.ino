#define minvalue 0
#define maxvalue 10

class Rotary {
  private:
    float s=0;
    
    float checkrotary(int rotary1, int rotary2){
    static byte old=0;
    byte new1 = digitalRead(rotary1);
    byte new2 = digitalRead(rotary2);
    byte new_var = (new1*2)|(new2^new1);
    byte d = old-new_var;
    old=new_var;
    switch(d){
      case 1: return -0.25;
      case 3: return 0.25;
      case 255: return 0.25;
      case 253: return -0.25;
      default: return 0;
      }
    }

  public:

    void printRotary() {
      while (true){
        float d = checkrotary(6, 7);
        s=s+d;
  
        if(d){Serial.println(int(s));}
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
