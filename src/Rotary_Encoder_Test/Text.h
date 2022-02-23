#define buttonpin 5

class Text {
  private:
    String text = "";

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
    
  public:
    String getText() {
      return text;
    }

    void updateButton() {
      if (digitalRead(buttonpin) == 0){
        do {} while (digitalRead(buttonpin) == 0);
        delay(50);
        int start_time = millis();
        int end_time = start_time + 200;
        bool double_click = false;
        do {
          if (digitalRead(buttonpin) == 0){
            double_click = true;
          }
        } while (millis() < end_time);

        if (double_click) {
          appendToText(kb_upper[rotary_encoder.getRotary()][0]);
          do {} while (digitalRead(buttonpin) == 0);
        }
        else{
          appendToText(kb_lower[rotary_encoder.getRotary()][0]);
        }
      }
    }
};

Text note;
