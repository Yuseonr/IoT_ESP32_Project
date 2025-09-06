#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>

#define NUM_LEDS_STRIP1 20
#define NUM_LEDS_STRIP2 20
#define NUM_LEDS_STRIP3 20

#define DATA_PIN1 23
#define DATA_PIN2 22
#define DATA_PIN3 21

CRGB strip1[NUM_LEDS_STRIP1];
CRGB strip2[NUM_LEDS_STRIP2];
CRGB strip3[NUM_LEDS_STRIP3];

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

WiFiClient espClient;
PubSubClient client(espClient);

// Pattern Procedure

int current_pattern = 0;

void rainbowPattern(CRGB* strip, int numLeds) {
    static uint8_t startHue = 0;
    startHue += 2;
    
    for(int i = 0; i < numLeds; i++) {
        strip[i] = CHSV(startHue + (i * 10), 255, 255);
    }
}

void snakePattern(CRGB* strip1, int numLeds1, CRGB* strip2, int numLeds2, CRGB* strip3, int numLeds3) {
    static int head = 0;
    static int totalLeds = numLeds1 + numLeds2 + numLeds3;
    const int SNAKE_LENGTH = 8; 
    
    // Clear all strips
    for(int i = 0; i < numLeds1; i++){strip1[i] = CRGB::Black;}
    for(int i = 0; i < numLeds2; i++){strip2[i] = CRGB::Black;}
    for(int i = 0; i < numLeds3; i++){strip3[i] = CRGB::Black;}
    
    for(int i = 0; i < SNAKE_LENGTH; i++) {
        int pos = (head - i + totalLeds) % totalLeds;
        if(pos < numLeds1) {
            // LED is on strip 1
            strip1[pos] = CRGB(255, 0, 0);
        }
        else if(pos < numLeds1 + numLeds2) {
			 // LED is on strip 2
            int strip2_pos = (numLeds2 - 1) - (pos - numLeds1);
            strip2[strip2_pos] = CRGB(255, 0, 0);
        }
        else {
			 // LED is on strip 3
            strip3[pos - numLeds1 - numLeds2] = CRGB(255, 0, 0);
        }
    }
    head = (head + 1) % totalLeds;
}

void fastPattern(CRGB* strip1, int numLeds1, CRGB* strip2, int numLeds2, CRGB* strip3, int numLeds3) {
	static int head1 = 0;
	static int head2 = 0;
	static int head3 = 0;

    for(int i = 0; i < numLeds1; i++){strip1[i] = CRGB::Black;}
    for(int i = 0; i < numLeds2; i++){strip2[i] = CRGB::Black;}
    for(int i = 0; i < numLeds3; i++){strip3[i] = CRGB::Black;}

    for(int i = 0; i <= 5; i++) {
        int pos = (head1 - i + numLeds1) % numLeds1;
        strip1[pos] = CRGB(0,0,255);
    }
	for(int i = 0; i <= 5; i++) {
        int pos = (head2 - i + numLeds2) % numLeds2;
        strip2[pos] = CRGB(0,0,255);
    }
	for(int i = 0; i <= 5; i++) {
        int pos = (head3 - i + numLeds3) % numLeds3;
        strip3[pos] = CRGB(0,0,255);
    }
    head1 = (head1 + 1) % numLeds1;
	head2 = (head2 + 1) % numLeds2;
	head3 = (head3 + 1) % numLeds3;
}

void arrowPattern(CRGB* strip1, int numLeds1, CRGB* strip2, int numLeds2, CRGB* strip3, int numLeds3){
	static int head1 = 0;
	static int head2 = 0;
	static int head3 = 0;

    for(int i = 0; i < numLeds1; i++){strip1[i] = CRGB::Black;}
    for(int i = 0; i < numLeds2; i++){strip2[i] = CRGB::Black;}
    for(int i = 0; i < numLeds3; i++){strip3[i] = CRGB::Black;}

    for(int i = 2; i <= 7; i++) {
        int pos = (head1 - i + numLeds1) % numLeds1;
        strip1[pos] = CRGB(0,255,0);
    }
	for(int i = 0; i <=5; i++) {
        int pos = (head2 - i + numLeds2) % numLeds2;
        strip2[pos] = CRGB(0,255,0);
    }
	for(int i = 2; i <= 7; i++) {
        int pos = (head3 - i + numLeds3) % numLeds3;
        strip3[pos] = CRGB(0,255,000);
    }
    head1 = (head1 + 1) % numLeds1;
	head2 = (head2 + 1) % numLeds2;
	head3 = (head3 + 1) % numLeds3;
}

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
    rainbowPattern(strip1, NUM_LEDS_STRIP1);
	  rainbowPattern(strip2, NUM_LEDS_STRIP2);
	  rainbowPattern(strip3, NUM_LEDS_STRIP3);  
    Serial.println("LED ON");
  } else if (messageTemp == "0") {
    rainbowPattern(strip1, NUM_LEDS_STRIP1);
	  rainbowPattern(strip2, NUM_LEDS_STRIP2);
	  rainbowPattern(strip3, NUM_LEDS_STRIP3);   
    Serial.println("LED OFF");
  } else if (messageTemp == "2") {
    rainbowPattern(strip1, NUM_LEDS_STRIP1);
	  rainbowPattern(strip2, NUM_LEDS_STRIP2);
	  rainbowPattern(strip3, NUM_LEDS_STRIP3);
    Serial.println("RAINBOW PATTERN");
  } else if (messageTemp == "3") {
    snakePattern(strip1, NUM_LEDS_STRIP1, strip2, NUM_LEDS_STRIP2, strip3, NUM_LEDS_STRIP3);
    Serial.println("SNAKE PATTERN");
  } else if (messageTemp == "4") {
    fastPattern(strip1, NUM_LEDS_STRIP1, strip2, NUM_LEDS_STRIP2, strip3, NUM_LEDS_STRIP3);
    Serial.println("FAST PATTERN");
  } else if (messageTemp == "5") {
    arrowPattern(strip1, NUM_LEDS_STRIP1, strip2, NUM_LEDS_STRIP2, strip3, NUM_LEDS_STRIP3);
    Serial.println("ARROW PATTERN");
  }
  FastLED.show();
	delay(20);

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
  Serial.begin(115200);
  FastLED.addLeds<WS2812, DATA_PIN1, RGB>(strip1, NUM_LEDS_STRIP1);
  FastLED.addLeds<WS2812, DATA_PIN2, RGB>(strip2, NUM_LEDS_STRIP2);
  FastLED.addLeds<WS2812, DATA_PIN3, RGB>(strip3, NUM_LEDS_STRIP3);
  FastLED.setBrightness(255);

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
