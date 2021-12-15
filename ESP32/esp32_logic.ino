const int RELAY_PIN1 = 5;  //Relay
const int RELAY_PIN2 = 4;  //Relay
const int buttonPin1 = 3; //Push_button1
const int buttonPin2 = 2; //Push_button2
unsigned long button_time1 = 0;
unsigned long last_button_time1 = 0;
unsigned long button_time2 = 0;
unsigned long last_button_time2 = 0;
bool state1 = false;
bool state2 = false;


void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin1), button_ISR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), button_ISR2, CHANGE);
}

void loop() {
  switch (state1) {
  case true:
    digitalWrite(RELAY_PIN1, LOW);
    Serial.println("Relay1 On");
    break;
  default:
    digitalWrite(RELAY_PIN1, HIGH);
    Serial.println("Relay1 Off");
    break;
}

  switch (state2) {
  case true:
    digitalWrite(RELAY_PIN2, LOW);
    Serial.println("Relay1 On");
    break;
  default:
    digitalWrite(RELAY_PIN2, HIGH);
    Serial.println("Relay1 Off");
    break;
}

}

void button_ISR1(){
  button_time1 = millis();
  if (button_time1 - last_button_time1 > 250){
    last_button_time1 = button_time1;
    state1 = !state1;
  }
}

void button_ISR2(){
  button_time2 = millis();
  if (button_time2 - last_button_time2 > 250){
    last_button_time2 = button_time2;
    state2 = !state2;
  }
}
