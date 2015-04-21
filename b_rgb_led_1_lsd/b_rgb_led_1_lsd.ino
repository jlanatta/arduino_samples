#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>

LCD *lcd = new LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  // put your setup code here, to run once:
  
  lcd->begin(20, 4);
  lcd->print("hola");
  lcd->clear();
//  lcd->print("chau!");
}

void loop() {
  // put your main code here, to run repeatedly:
}
