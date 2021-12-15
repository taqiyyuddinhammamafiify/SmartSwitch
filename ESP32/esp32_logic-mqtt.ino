#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

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
const char *TOPIC = "ESP1";  // Ganti dengan topic yang ingin disubscribe

const int port = 443;

// Ukuran payload, ganti ke ukuran yang diinginkan
StaticJsonDocument<200> doc;
char data[80];
String lampu1 = "0";
String lampu2 = "0";

WiFiClient espClient;
PubSubClient client(espClient);

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if(client.connect(ID)) {
      client.subscribe(TOPIC);
      Serial.println("connected");
      Serial.print("Subcribed to: ");
      Serial.println(TOPIC);
      Serial.println('\n');

    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
   // handle message arrived
   char inData[80];
   
   Serial.print("payload: ");
   for(int i = 26; i < length; i++){
      // Serial.print((char)payload[i]);
      inData[(i - 26)] = (char)payload[i];
   }
   
   Serial.println();
   deserializeJson(doc, payload);
   
   // Pull the value from key "VALUE" from the JSON message {"value": 1 , "someOtherValue" : 200}, ganti ke sensor pada device
   String val1 = doc["lampu1"];
   String val2 = doc["lampu2"];
   lampu1 = val1;
   lampu2 = val2;

   // Kondisi untuk smart switch, ganti ke digital write atau lainnya
   /*if (true) {
      
   } else if (false) {
     
   }
}*/


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
   client.setServer(mqtt_server, 1883);
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
   
   String payload = "{\"lampu1\": " + String(lampu1) + ", \"lampu2\": " + String(lampu2) + "}";
   payload.toCharArray(data, (payload.length() + 1));
   client.publish(TOPIC, data);
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
