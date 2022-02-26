class Rotary {
  private:
    String oldRotaryLetters;
    float s=0;
    String text = " - ";
    int mode = 0;
    unsigned int last_pressed_time = 0;
    
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

    String getMode() {
      return modes[mode];
    }

    void appendToText(String appending_text) {
      if (appending_text == "BS") {
        String old_text = text;
        int old_text_length = old_text.length();
        text = old_text.substring(0, old_text_length - 1);
      }
      else if (appending_text == "RE") {
        text += "\n - ";
        mode = 0; //When new line, set kb to letters
      }
      else {
        text += appending_text;
      }
      print_info();
    }
  
    int getRotary() {
      float d = checkrotary(6, 7);
      s=s+d;

      if (s>kb1_size){s = 0;}
      if (s<0){s = kb1_size;}

      return int(s);
    }

    String getRotaryLetters(){
      int turn_value = getRotary();
      String kb_value;
      if (mode == 0) {
        kb_value = kb_1[turn_value][0];
      }
      else if (mode == 1) {
        kb_value = kb_2[turn_value][0];
      }
      else if (mode == 2) {
        kb_value = kb_3[turn_value][0];
      }
      return kb_value;
    }



    void updateButtons() {
      if (digitalRead(typepin) == 0){
        if ((last_pressed_time + 500) <= millis()) {
          if (mode == 0) {
            appendToText(kb_1[getRotary()][0]);
          }
          else if (mode == 1) {
            appendToText(kb_2[getRotary()][0]);
          }
          else if (mode == 2) {
            appendToText(kb_3[getRotary()][0]);
          }
          do {} while (digitalRead(typepin) == 0);

          last_pressed_time = millis();
        }
      }
      else if (digitalRead(modepin) == 0){
        mode++;
        if (mode >= num_of_modes){mode = 0;}
        do {} while (digitalRead(modepin) == 0);
      }
    }

    void print_info() {
      Serial.print("Mode: ");
      Serial.print(modes[mode]);
      Serial.print(" | Character: ");
      Serial.println(getRotaryLetters());
      Serial.println(text);
    }

  public:
    void getTypedLetters() {
      String oldMode = getMode();
      String newRotaryLetters = getRotaryLetters();
      if (oldRotaryLetters != newRotaryLetters){
        print_info();
        oldRotaryLetters = newRotaryLetters;
      }
      updateButtons();
    }
    
    void begin() {
      pinMode(typepin, INPUT_PULLUP);
      pinMode(modepin, INPUT_PULLUP);
      pinMode(6, INPUT);
      pinMode(7, INPUT);
    }
};
