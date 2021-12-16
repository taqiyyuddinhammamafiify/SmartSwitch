#include <WiFi.h>
#include <PubSubClient.h>

//Logic ESP
const int RELAY_PIN1 = 33; //5;  //Relay
const int RELAY_PIN2 = 32; //4;  //Relay
const int buttonPin1 = 26; //3; //Push_button1
const int buttonPin2 = 25; //Push_button2
unsigned long button_time1 = 0;
unsigned long last_button_time1 = 0;
unsigned long button_time2 = 0;
unsigned long last_button_time2 = 0;
bool state1 = false;
bool state2 = false;

// Ganti dengan ssid dan password wifi
const char* ssid = "SSID";
const char* password = "PASSWORD";

const char* server = "node-red.hfrk.de";
const char* mqtt_server = "20.63.210.119";
const char *ID = "ESP_Alex";  // Ganti dengan nama device
const char *TOPIC_1 = "ESP1/Lampu1";  // Ganti dengan topic yang ingin disubscribe
const char *TOPIC_2 = "ESP1/Lampu2";
const char *TOPIC_3 = "TIMER";

const int mqtt_port = 1883;
char data[80];

WiFiClient espClient;
PubSubClient client(espClient);

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if(client.connect(ID)) {
      client.subscribe(TOPIC_1);
      client.subscribe(TOPIC_2);
      client.subscribe(TOPIC_3);
      Serial.println("connected");
      Serial.print("Subcribed to: ");
      Serial.println(TOPIC_1);
      Serial.println(TOPIC_2);
      Serial.println(TOPIC_3);
      Serial.println('\n');

    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strncmp(topic, "TIMER", 5) == 0) {
    // TIMER logic
    if (strncmp((char*)payload, "150", 3) == 0) {
      // set timer ke 150 menit
      Serial.println("Timer diset ke 150 menit");
    }
    else if (strncmp((char*)payload, "120", 3) == 0) {
      // set timer ke 120 menit
      Serial.println("Timer diset ke 120 menit");
    }
    else if (strncmp((char*)payload, "90", 2) == 0) {
      // set timer ke 90 menit
      Serial.println("Timer diset ke 90 menit");
    }
    else if (strncmp((char*)payload, "60", 2) == 0) {
      // set timer ke 60 menit
      Serial.println("Timer diset ke 60 menit");
    }
  }
  else if (strncmp(topic, "ESP1/Lampu1", 11) == 0) {
    if (strncmp((char*)payload, "true", 4) == 0} 
      state1 = true;
    else
      state1 = false;
  }
  else if (strncmp(topic, "ESP1/Lampu2", 11) == 0) {
    if (strncmp((char*)payload, "true", 4) == 0} 
      state2 = true;
    else
      state2 = false;
  }
}


void setup() {
  //Logic ESP
  Serial.begin(115200);
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin1), button_ISR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), button_ISR2, CHANGE);
   //Serial.begin(115200);
   WiFi.mode(WIFI_STA);                    //The WiFi is in station mode. The    other is the softAP mode
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
   }
   Serial.println("");  Serial.print("WiFi connected to: "); Serial.println(ssid);  Serial.println("IP address: ");  Serial.println(WiFi.localIP());
   client.setServer(mqtt_server, mqtt_port);
   client.setCallback(callback);
   delay(2000);
}

void loop() {
  //Logic ESP32
    switch (state1) {
  case true:
    digitalWrite(RELAY_PIN1, HIGH);
    Serial.println("Relay1 On");
    break;
  default:
    digitalWrite(RELAY_PIN1, LOW);
    Serial.println("Relay1 Off");
    break;
}

  switch (state2) {
  case true:
    digitalWrite(RELAY_PIN2, HIGH);
    Serial.println("Relay2 On");
    break;
  default:
    digitalWrite(RELAY_PIN2, LOW);
    Serial.println("Relay2 Off");
    break;
}
   if (!client.connected()) {
    mqttconnect();
   }
   // This sends off your payload. ganti dengan payload yang diinginkan
   
   // Publish state1
   String payload = state1 ? "true" : "false";
   payload.toCharArray(data, (payload.length() + 1));
   client.publish(TOPIC_1, data);

   // Publish state2
   payload = state2 ? "true" : "false";
   payload.toCharArray(data, (payload.length() + 1));
   client.publish(TOPIC_2, data);

   delay(500);
   client.loop();
}

//Logic ESP
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
