#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "your ssid";
const char* password = "your password";
const char* mqtt_server = "public.mqtthq.com";
const char* mqtt_topic = "lamp";
const int LED_BUILTIN = 2 ;
const int led = 11 ;
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // Connect to WiFi network
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Message arrived:");
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
// topic, led on-off
  if (strcmp(topic, mqtt_topic) == 0) {
    if (payload[0] == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED turned on");
    } else if (payload[0] == '0') {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("LED turned off");
    }else if (payload[0] == '2') {
      digitalWrite(led, HIGH);
      Serial.println("led on");
    }else if (payload[0] == '3') {
      digitalWrite(led, LOW);
      Serial.println("led off");
    }
    
  }
}
//reconnect
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
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
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(led, OUTPUT);
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