#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>

#define RED_PIN 3
#define GREEN_PIN 6
#define BLUE_PIN 5

LCD *lcd = new LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int pins[3] = {RED_PIN, GREEN_PIN, BLUE_PIN};
int values[3] = {0, 0, 0};
int previousValues[3] = {0, 0, 0};

int index = 0;
bool directionUp = true;

EthernetServer server(1000);

void setup() {
  Serial.begin(9600);
  lcd->begin(20, 4);
  
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  Ethernet.begin(mac, IPAddress(192, 168, 1, 177));

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  lcd->print(F("IP: "));
  
  lcd->setCursor(0,1);
  lcd->print(F("Red  :0"));
  lcd->setCursor(0,2);
  lcd->print(F("Green:0"));
  lcd->setCursor(0,3);
  lcd->print(F("Blue :0"));
}

void loop() {
  webLoop();
  
  for (int index = 0; index < 3; index ++) {
    if (previousValues[index] != values[index]) {
      analogWrite(pins[index], values[index]);
      lcd->setCursor(6, index + 1);
      lcd->print(values[index]);
      lcd->print(F("   "));
      previousValues[index] = values[index];
    }
  }
  
  delay(20);
}

void webLoop() {
  EthernetClient client = server.available();
  String command = "";
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      char c = NULL;
      if (client.available()) {
        c = client.read();
        command += c;
        if (c == '\n' || c == '\r') {
          char cmd[20] = {0};
          command.toCharArray(cmd, command.length());
          sscanf(cmd, "%d;%d;%d", &values[0], &values[1], &values[2]);
          break;
        }
//        command += c;
      }
    }
    delay(1);
    // close the connection:
    client.stop();
  }
}
