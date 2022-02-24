class Rotary {
  private:
    String oldRotaryLetters;
    float s=0;
    String text = "";
    
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

    void appendToText(String appending_text) {
      if (appending_text == "BS") {
        String old_text = text;
        int old_text_length = old_text.length();
        text = old_text.substring(0, old_text_length - 1);
      }
      else {
        text += appending_text;
      }
      Serial.print("Text: '");
      Serial.print(text);
      Serial.println("'");
    }
  
    int getRotary() {
      float d = checkrotary(6, 7);
      s=s+d;

      if (s>kb1_size-0.75){s = 0;}
      if (s<0){s = kb1_size;}

      return int(s);
    }

    String getRotaryLetters(String letter_list){
      int turn_value = getRotary();
      String kbl_value = kb_lower_1[turn_value][0];
      String kbu_value = kb_upper_1[turn_value][0];
      if (letter_list == "Lower") {
        return kbl_value;
      }
      else if (letter_list == "Upper") {
        return kbu_value;
      }
      return kbl_value + " - " + kbu_value;
    }
    
    String getText() {
      return text;
    }

    void updateButton() {
      if (digitalRead(typepin) == 0){
        do {} while (digitalRead(typepin) == 0);
        delay(50);
        unsigned int start_time = millis();
        unsigned int end_time = start_time + 200;
        bool double_click = false;
        do {
          if (digitalRead(typepin) == 0){
            double_click = true;
          }
        } while (millis() < end_time);

        if (double_click) {
          appendToText(kb_upper_1[getRotary()][0]);
          do {} while (digitalRead(typepin) == 0);
        }
        else{
          appendToText(kb_lower_1[getRotary()][0]);
        }
      }
    }

  public:
    void getTypedLetters() {
      String newRotaryLetters = getRotaryLetters("Both");
      if (oldRotaryLetters != newRotaryLetters){
        Serial.println(newRotaryLetters);
        oldRotaryLetters = newRotaryLetters;
      }
      updateButton();
    }
    
    void begin() {
      pinMode(typepin, INPUT_PULLUP);
      pinMode(modepin, INPUT_PULLUP);
      pinMode(6, INPUT);
      pinMode(7, INPUT);
    }
};
