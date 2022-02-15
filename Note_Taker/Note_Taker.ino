#define minvalue 0
#define maxvalue 43

char text = "";

const String kb_lower[][1] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ",", ".", "-", "=", "/", "[", "]", " ",};
const String kb_upper[][1] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "<", ">", "_", "+", "|", "{", "}", " ",};

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
        case 1: return 0.25;
        case 3: return -0.25;
        case 255: return -0.25;
        case 253: return 0.25;
        default: return 0;
        }
    }

  public:
    int getRotary() {
      float d = checkrotary(6, 7);
      s=s+d;

      if (s>maxvalue){s = minvalue;}
      if (s<minvalue){s = maxvalue;}

      return int(s);
    }

    String getRotaryLetter(){
      int turn_value = getRotary();
      String kbl_value = kb_lower[turn_value][0];
      String kbu_value = kb_upper[turn_value][0];
      return kbl_value + " - " + kbu_value;
    }
};

Rotary rotary_encoder;

void setup() {
  // put your setup code here, to run once:
  //Open the serial port
  Serial.begin(9600);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
}

String oldRotaryLetter = "";

void loop() {
  String newRotaryLetter = rotary_encoder.getRotaryLetter();
  if (oldRotaryLetter != newRotaryLetter){
    Serial.println(newRotaryLetter);
    oldRotaryLetter = newRotaryLetter;
  }
}
