#define PIN_KEYBOARD_UP 10
#define PIN_KEYBOARD_DOWN 11
#define PIN_KEYBOARD_LEFT 12
#define PIN_KEYBOARD_RIGHT 13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  int lastKey = -1;
  int keyIndex = readKey();
  if (keyIndex > 0) {
    switch(keyIndex) {
      case 1:
        Serial.println("UP");
        break;
      case 2:
        Serial.println("DOWN");
        break;
      case 3:
        Serial.println("RIGHT");
        break;
      case 4:
        Serial.println("LEFT");
        break;
    }
  }
  delay(250);
}


int lastKey = 0;
int readKey() {
  bool keys[4] = {digitalRead(PIN_KEYBOARD_UP), digitalRead(PIN_KEYBOARD_DOWN), digitalRead(PIN_KEYBOARD_RIGHT), digitalRead(PIN_KEYBOARD_LEFT)};
    
  for (int i=0;i<4;i++) {
    if (keys[i] == LOW) {
      if (lastKey == i+1) {
        return 0;
      } else {
        lastKey = i+1;
        return lastKey;
      }
    }
  }
  lastKey = 0;
  return 0;
}
