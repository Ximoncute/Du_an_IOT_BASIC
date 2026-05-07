#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi và MQTT
const char* ssid = "P502";
const char* password = "88888888";
const char* mqtt_server = "broker.emqx.io";
const char* mqtt_topic_temp = "home/sensor/temp";
const char* mqtt_topic_hum = "home/sensor/hum";
const char* mqtt_topic_ldr = "home/sensor/ldr";
const char* mqtt_topic_led1 = "home/led1";
const char* mqtt_topic_led2 = "home/led2";
const char* mqtt_topic_led3 = "home/led3";
const char* mqtt_topic_mode = "home/mode";

WiFiClient espClient;
PubSubClient client(espClient);

// Chân cảm biến
#define DHTPIN 4      // GPIO4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define LDRPIN 16     // GPIO16
#define LED1_PIN 0    // GPIO0
#define LED2_PIN 4    // GPIO4
#define LED3_PIN 12   // GPIO12

bool autoMode = true;

void setup() {
  Serial.begin(115200);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  
  dht.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  
  if (String(topic) == mqtt_topic_mode) {
    autoMode = (msg == "auto");
  }
  
  if (!autoMode) {
    if (String(topic) == mqtt_topic_led1) digitalWrite(LED1_PIN, msg == "on");
    if (String(topic) == mqtt_topic_led2) digitalWrite(LED2_PIN, msg == "on");
    if (String(topic) == mqtt_topic_led3) digitalWrite(LED3_PIN, msg == "on");
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      client.subscribe(mqtt_topic_led1);
      client.subscribe(mqtt_topic_led2);
      client.subscribe(mqtt_topic_led3);
      client.subscribe(mqtt_topic_mode);
    } else delay(5000);
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int ldrValue = analogRead(LDRPIN);
  bool ldrDark = (ldrValue < 500);
  
  if (autoMode) {
    digitalWrite(LED1_PIN, t > 30);
    digitalWrite(LED2_PIN, h > 70);
    digitalWrite(LED3_PIN, ldrDark);
  }
  
  client.publish(mqtt_topic_temp, String(t).c_str());
  client.publish(mqtt_topic_hum, String(h).c_str());
  client.publish(mqtt_topic_ldr, ldrDark ? "dark" : "light");
  
  delay(2000);
}