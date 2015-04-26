#define LED_A 5
#define LED_B 6
#define LED_C 9

int pin_matrix[6][2] {
{LED_B, LED_A},
{LED_A, LED_B},
{LED_C, LED_B},
{LED_B, LED_C},
{LED_C, LED_A},
{LED_A, LED_C},
};
  
//int ledState[] = {LOW, LOW, HIGH, LOW, LOW, LOW};
int ledState[6] = {LOW};
int ledCount = 6;
int ledIndex = 0;

void setup()
{
  reset_pins();
  doLightsCycle();    
}

int last_time = 0;
int count = 0;
bool blocked = false;
void loop()
{
//  int current_time = millis();
//  if ((current_time - last_time) > 100) {
//    ledState[ledIndex] = LOW;
//    ledIndex ++;
//    if (ledIndex == 6) {
//      ledIndex = 0;
//    }
//    ledState[ledIndex] = HIGH;
//    last_time = current_time;
//  }

  if (count < 50) {
    ledState[0] = HIGH;
    ledState[1] = HIGH;
    ledState[2] = HIGH;
    ledState[3] = HIGH;
    ledState[4] = HIGH;
    ledState[5] = HIGH;    
    count++;
  } else {
    ledState[0] = LOW;
    ledState[1] = LOW;
    ledState[2] = LOW;    
    ledState[3] = LOW;
    ledState[4] = LOW;
    ledState[5] = LOW;    
  }

  doLightsCycle();

}

void doLightsCycle() {
  for (int i = 0; i<ledCount; i++) {
    if (ledState[i] == HIGH) {
      set_pins(i);
    }
  }
}

void reset_pins()
{
  pinMode(LED_A, INPUT);
  pinMode(LED_B, INPUT);
  pinMode(LED_C, INPUT);
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_C, LOW);
}

void set_pins(int pin_index) {
  reset_pins();

  int high_pin = pin_matrix[pin_index][0];
  int low_pin = pin_matrix[pin_index][1];

  // set the high and low pins to output
  pinMode(high_pin, OUTPUT);
  pinMode(low_pin, OUTPUT);

  // set logic as required
  digitalWrite(high_pin, HIGH);
  digitalWrite(low_pin, LOW);
}
