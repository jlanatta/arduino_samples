#include <Time.h>
#include <Timer.h>
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#define PIN_OUT 2
#define PIN_IN A0

LCD *lcd = new LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
EthernetServer server(888);
IPAddress timeServer(132, 163, 4, 101); // time-a.timefreq.bldrdoc.gov
const int timeZone = -3;
EthernetUDP Udp;
unsigned int localPort = 8888;
Timer timer;

String commandBuffer[] = {"", "", ""};
int bufferCount=0;

void setup() {
  lcd->begin(20,4);
  lcd->print(F("Initializing..."));

  Serial.begin(9600);
  pinMode(PIN_OUT, OUTPUT);  
  pinMode(PIN_IN, INPUT);  
  digitalWrite(PIN_OUT, HIGH);
  
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  Ethernet.begin(mac, IPAddress(192, 168, 1, 177));
  server.begin();

  lcd->setCursor(0,1);
  lcd->print(F("IP: "));
  lcd->print(Ethernet.localIP());

  lcd->setCursor(0,2);
  lcd->print(F("Retrieving time..."));

  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  while(timeStatus() == timeNotSet) {}
  
  delay(309v           g56 cvk,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,lkl,,,,,,,.000);

  lcd->clear();
  lcd->print("Port:888");
  
  appendCommand(F("Waiting"));
  printTime();
  timer.every(1000, printTime);
}

void loop() {
  webLoop();
  timer.update();
}

void printTime() {
  String time = currentTime();
  lcd->setCursor(12,0);
  lcd->print(time);
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
        if (c == '\n' || c == '\r') {
          appendCommand(command);
          if (command.equals("on")) {
            digitalWrite(PIN_OUT, LOW);
          } else if (command.equals("off")) {
            digitalWrite(PIN_OUT, HIGH);
          }
          break;
        }
        command += c;
      }
    }
    delay(1);
    // close the connection:
    client.stop();
  }
}

void appendCommand(String command) {
  String line = currentTime();
  lcd->setCursor(12,0);
  lcd->print(line);

  lcd->clear();
  printTime();

  line += " ";
  line += command;
  
  if (bufferCount == 3) {
    commandBuffer[0] = commandBuffer[1];
    commandBuffer[1] = commandBuffer[2];
    commandBuffer[2] = line;
  } else {
    commandBuffer[bufferCount] = line;
    bufferCount ++;
  }
  
  for (int i = 0; i < 3; i++) {
      lcd->setCursor(0,i+1);
      lcd->print(commandBuffer[i]);
  }
}

String currentTime() {
  int h = hour();
  int m = minute();
  int s = second();
  
  String time = "";
  time += h;
  time += ":";
  if (m < 10) {
    time += "0";
  }
  time += m;
  time += ":";
  if (s < 10) {
    time += "0";
  }
  time += s;
  return time;
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

