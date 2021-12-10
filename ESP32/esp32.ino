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
   deserializeJson(doc, inData);
   
   // Pull the value from key "VALUE" from the JSON message {"value": 1 , "someOtherValue" : 200}
   String val = doc["lampu1"];
   // DO SOMETHING WITH THE DATA THAT CAME IN!
   Serial.println(val); 

   // Kondisi untuk smart switch, ganti ke digital write atau lainnya
   if (val == "1") {
      
   } else if (val == "0") {
     
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
   String lampu1 = "1";
   String lampu2 = "0";
   String payload = "{\"lampu1\": " + lampu1 + ", \"lampu2\": " + lampu2 + "}";
   payload.toCharArray(data, (payload.length() + 1));
   client.publish(TOPIC, data);
   delay(500);
   client.loop();
}
