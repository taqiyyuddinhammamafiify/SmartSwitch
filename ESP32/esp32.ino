#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

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
   if (true) {
      
   } else if (false) {
     
   }
}


void setup() {
   Serial.begin(115200);
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
