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
const char* topic_mode = "esp8266/mode";

// ========== CHÂN CẮM ==========
#define DHTPIN 4        // DHT11 - GPIO4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define LDRPIN 16        // LDR - D0

#define LED1_PIN 0       // LED1 - GPIO0
#define LED2_PIN 5       // LED2 - GPIO5
#define LED3_PIN 12      // LED3 - GPIO12

// ========== BIẾN TOÀN CỤC ==========
WiFiClient espClient;
PubSubClient client(espClient);
bool autoMode = true;
unsigned long lastPublish = 0;
const long publishInterval = 2000;

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
  
  // Kết nối WiFi
  Serial.print("Đang kết nối WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi đã kết nối!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  // Kết nối MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  connectMQTT();
}

void connectMQTT() {
  while (!client.connected()) {
    if (client.connect("ESP8266_Client")) {
      Serial.println("MQTT đã kết nối!");
      client.subscribe(topic_led1);
      client.subscribe(topic_led2);
      client.subscribe(topic_led3);
      client.subscribe(topic_mode);
    } else {
      Serial.print("MQTT lỗi, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

// ========== CALLBACK NHẬN LỆNH ==========
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  String topicStr = String(topic);
  
  if (topicStr == topic_mode) {
    if (message == "auto") {
      autoMode = true;
      Serial.println("Chế độ TỰ ĐỘNG");
    } else if (message == "manual") {
      autoMode = false;
      Serial.println("Chế độ THỦ CÔNG");
    }
  }
  
  // Điều khiển LED ở chế độ thủ công
  if (!autoMode) {
    if (topicStr == topic_led1) {
      digitalWrite(LED1_PIN, message == "1" ? HIGH : LOW);
      Serial.print("LED1: ");
      Serial.println(message == "1" ? "BẬT" : "TẮT");
    }
    else if (topicStr == topic_led2) {
      digitalWrite(LED2_PIN, message == "1" ? HIGH : LOW);
      Serial.print("LED2: ");
      Serial.println(message == "1" ? "BẬT" : "TẮT");
    }
    else if (topicStr == topic_led3) {
      digitalWrite(LED3_PIN, message == "1" ? HIGH : LOW);
      Serial.print("LED3: ");
      Serial.println(message == "1" ? "BẬT" : "TẮT");
    }
  }
}

// ========== ĐIỀU KHIỂN LED TỰ ĐỘNG ==========
void autoControlLEDs(float temp, float hum, int ldrValue) {
  // LED1: Bật khi nhiệt độ > 30°C
  if (temp > 30) {
    digitalWrite(LED1_PIN, HIGH);
  } else {
    digitalWrite(LED1_PIN, LOW);
  }
  
  // LED2: Bật khi độ ẩm > 70%
  if (hum > 70) {
    digitalWrite(LED2_PIN, HIGH);
  } else {
    digitalWrite(LED2_PIN, LOW);
  }
  
  // LED3: Bật khi LDR tối (giá trị < 500)
  if (ldrValue < 500) {
    digitalWrite(LED3_PIN, LOW);
  } else {
    digitalWrite(LED3_PIN, HIGH);
  }
}

// ========== VÒNG LẶP CHÍNH ==========
void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();
  
  unsigned long now = millis();
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;
    
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int ldrValue = analogRead(LDRPIN);
    
    // Chỉ xác định trạng thái SÁNG hoặc TỐI
    String ldrStatus = (ldrValue < 500) ? "Sáng" : "Tối";
    
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Lỗi đọc DHT11!");
      return;
    }
    
    char tempStr[10], humStr[10];
    dtostrf(temperature, 4, 1, tempStr);
    dtostrf(humidity, 4, 1, humStr);
    
    client.publish(topic_temp, tempStr);
    client.publish(topic_hum, humStr);
    client.publish(topic_ldr, ldrStatus.c_str());
    
    Serial.print("Nhiệt độ: ");
    Serial.print(temperature);
    Serial.print("°C | Độ ẩm: ");
    Serial.print(humidity);
    Serial.print("% | LDR: ");
    Serial.println(ldrStatus);
    
    if (autoMode) {
      autoControlLEDs(temperature, humidity, ldrValue);
    }
  }
}