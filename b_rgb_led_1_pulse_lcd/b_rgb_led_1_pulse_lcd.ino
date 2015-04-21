#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>

#define RED_PIN 3
#define GREEN_PIN 6
#define BLUE_PIN 5

LCD *lcd = new LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int pins[3] = {RED_PIN, GREEN_PIN, BLUE_PIN};
int values[3] = {0, 0, 0};
bool directions[3] = {true, true, true};

int index = 0;
bool directionUp = true;

void setup() {
  lcd->begin(20, 4);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  lcd->setCursor(0,0);
  lcd->print(F("Red  :0"));
  lcd->setCursor(0,1);
  lcd->print(F("Green:0"));
  lcd->setCursor(0,2);
  lcd->print(F("Blue :0"));
}

void loop() {  
  analogWrite(pins[index], values[index]);
  lcd->setCursor(6, index);
  lcd->print(values[index]);
  lcd->print(F("   "));

  if ((values[index] == 255 && directions[index] == true) || (values[index] == 0 && directions[index] == false)) {
    directions[index] = !directions[index];
    index ++;
    if (index == 3) {
      index = 0;
    }
  }
  
  if (directions[index]) {
    values[index]++;
  } else {
    values[index]--;
  }

  delay(20);
}
