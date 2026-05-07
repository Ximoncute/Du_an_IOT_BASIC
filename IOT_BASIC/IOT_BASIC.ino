#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ========== CẤU HÌNH WiFi & MQTT ==========
const char* ssid = "Hieu Cute";
const char* password = "ximon123";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;

// ========== TOPIC MQTT ==========
const char* topic_temp = "esp8266/dht11/temperature";
const char* topic_hum = "esp8266/dht11/humidity";
const char* topic_ldr = "esp8266/ldr/status";
const char* topic_led1 = "esp8266/led1/control";
const char* topic_led2 = "esp8266/led2/control";
const char* topic_led3 = "esp8266/led3/control";
const char* topic_led1_status = "esp8266/led1/status";
const char* topic_led2_status = "esp8266/led2/status";
const char* topic_led3_status = "esp8266/led3/status";
const char* topic_mode = "esp8266/mode";
const char* topic_get_status = "esp8266/get/status";

// ========== CHÂN CẮM ==========
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define LDRPIN 16

#define LED1_PIN 0
#define LED2_PIN 5
#define LED3_PIN 12

// ========== BIẾN TOÀN CỤC ==========
WiFiClient espClient;
PubSubClient client(espClient);
bool autoMode = true;
unsigned long lastPublish = 0;
unsigned long lastLEDCheck = 0;
const long publishInterval = 2000;
const long ledCheckInterval = 100;

bool led1State = false;
bool led2State = false;
bool led3State = false;

float lastTemperature = 0;
float lastHumidity = 0;
int lastLdrValue = 0;

bool isUpdatingLED = false;  // Chống gửi trạng thái liên tục

// ========== KHỞI TẠO ==========
void setup() {
  Serial.begin(115200);
  
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  
  dht.begin();
  
  Serial.print("Dang ket noi WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi da ket noi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  connectMQTT();
}

void connectMQTT() {
  while (!client.connected()) {
    if (client.connect("ESP8266_Client")) {
      Serial.println("MQTT da ket noi!");
      client.subscribe(topic_led1);
      client.subscribe(topic_led2);
      client.subscribe(topic_led3);
      client.subscribe(topic_mode);
      client.subscribe(topic_get_status);
      
      delay(500);
      publishFullStatus();
    } else {
      Serial.print("MQTT loi, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void publishFullStatus() {
  client.publish(topic_led1_status, led1State ? "BAT" : "TAT");
  client.publish(topic_led2_status, led2State ? "BAT" : "TAT");
  client.publish(topic_led3_status, led3State ? "BAT" : "TAT");
  client.publish(topic_mode, autoMode ? "auto" : "manual");
  Serial.println("Da gui trang thai day du");
}

void setLED(int led, bool state, const char* source) {
  if (led == 1) {
    if (led1State != state) {
      led1State = state;
      digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
      Serial.print(source);
      Serial.print(" LED1: ");
      Serial.println(led1State ? "BAT" : "TAT");
      client.publish(topic_led1_status, led1State ? "BAT" : "TAT");
    }
  }
  else if (led == 2) {
    if (led2State != state) {
      led2State = state;
      digitalWrite(LED2_PIN, led2State ? HIGH : LOW);
      Serial.print(source);
      Serial.print(" LED2: ");
      Serial.println(led2State ? "BAT" : "TAT");
      client.publish(topic_led2_status, led2State ? "BAT" : "TAT");
    }
  }
  else if (led == 3) {
    if (led3State != state) {
      led3State = state;
      digitalWrite(LED3_PIN, led3State ? HIGH : LOW);
      Serial.print(source);
      Serial.print(" LED3: ");
      Serial.println(led3State ? "BAT" : "TAT");
      client.publish(topic_led3_status, led3State ? "BAT" : "TAT");
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  String topicStr = String(topic);
  
  // Xử lý yêu cầu lấy trạng thái
  if (topicStr == topic_get_status) {
    publishFullStatus();
    return;
  }
  
  // Xử lý chế độ
  if (topicStr == topic_mode) {
    if (message == "auto" && !autoMode) {
      autoMode = true;
      Serial.println("=== CHUYEN SANG CHE DO: TU DONG ===");
      client.publish(topic_mode, "auto");
      // Cập nhật LED theo cảm biến ngay lập tức
      autoControlLEDs();
    } 
    else if (message == "manual" && autoMode) {
      autoMode = false;
      Serial.println("=== CHUYEN SANG CHE DO: THU CONG ===");
      client.publish(topic_mode, "manual");
    }
    return;
  }
  
  // Ở chế độ thủ công mới nhận điều khiển LED
  if (!autoMode) {
    if (topicStr == topic_led1) {
      setLED(1, (message == "1"), "THU CONG");
    }
    else if (topicStr == topic_led2) {
      setLED(2, (message == "1"), "THU CONG");
    }
    else if (topicStr == topic_led3) {
      setLED(3, (message == "1"), "THU CONG");
    }
  }
}

void autoControlLEDs() {
  if (!autoMode) return;
  
  bool newLed1 = (lastTemperature > 30);
  bool newLed2 = (lastHumidity > 70);
  bool newLed3 = (lastLdrValue > 500);
  
  setLED(1, newLed1, "TU DONG");
  setLED(2, newLed2, "TU DONG");
  setLED(3, newLed3, "TU DONG");
}

void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();
  
  unsigned long now = millis();
  
  // Đọc cảm biến mỗi 2 giây
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;
    
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int ldrValue = analogRead(LDRPIN);
    
    if (!isnan(humidity) && !isnan(temperature)) {
      lastTemperature = temperature;
      lastHumidity = humidity;
      lastLdrValue = ldrValue;
      
      String ldrStatus = (ldrValue > 500) ? "Tối" : "Sáng";
      
      char tempStr[10], humStr[10];
      dtostrf(temperature, 4, 1, tempStr);
      dtostrf(humidity, 4, 1, humStr);
      
      client.publish(topic_temp, tempStr);
      client.publish(topic_hum, humStr);
      client.publish(topic_ldr, ldrStatus.c_str());
      
      Serial.print("Nhiet do: ");
      Serial.print(temperature);
      Serial.print("C | Do am: ");
      Serial.print(humidity);
      Serial.print("% | LDR: ");
      Serial.print(ldrValue);
      Serial.print(" (");
      Serial.print(ldrStatus);
      Serial.print(") | LED1: ");
      Serial.print(led1State ? "BAT" : "TAT");
      Serial.print(" | LED2: ");
      Serial.print(led2State ? "BAT" : "TAT");
      Serial.print(" | LED3: ");
      Serial.println(led3State ? "BAT" : "TAT");
    }
  }
  
  // Cập nhật LED tự động mỗi 100ms
  if (now - lastLEDCheck >= ledCheckInterval) {
    lastLEDCheck = now;
    if (autoMode) {
      autoControlLEDs();
    }
  }
}