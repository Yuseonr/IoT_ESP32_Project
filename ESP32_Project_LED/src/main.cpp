#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>


/*
  ESP32 MQTT LED Controller
  - Connects to WiFi
  - Subscribes to topic "IoT_Undip_ESP32"
  - Turns LED ON when receiving "1"
  - Turns LED OFF when receiving "0"
*/

// WiFi credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT broker
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "IoT_Undip_ESP32";

// GPIO pin for LED (you can use onboard LED on pin 2 for ESP32 DevKit)
const int ledPin = 2;

WiFiClient espClient;
PubSubClient client(espClient);

// connect to WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// handle incoming MQTT messages
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(" | Message: ");

  String messageTemp;
  for (unsigned int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);

  if (messageTemp == "1") {
    digitalWrite(ledPin, HIGH);  // Turn LED ON
    Serial.println("LED ON");
  } else if (messageTemp == "0") {
    digitalWrite(ledPin, LOW);   // Turn LED OFF
    Serial.println("LED OFF");
  }
}

// reconnect to MQTT broker if disconnected
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create unique client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
