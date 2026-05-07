# 🌡️ ESP8266 IoT - Hệ Thống Giám Sát & Điều Khiển Thông Minh

[![Platform](https://img.shields.io/badge/Platform-ESP8266-blue)](https://www.espressif.com/)
[![MQTT](https://img.shields.io/badge/MQTT-EMQX-orange)](https://www.emqx.io/)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)

## 📌 Giới Thiệu

Dự án **ESP8266 IoT** là hệ thống giám sát và điều khiển thông minh, cho phép:

- 📊 **Đọc cảm biến** nhiệt độ (DHT11), độ ẩm (DHT11), ánh sáng (LDR)
- 💡 **Điều khiển 3 LED** (bật/tắt thủ công hoặc tự động theo điều kiện)
- 🌐 **Giao diện Web** trực quan, hiển thị dữ liệu thời gian thực
- 📈 **Biểu đồ** nhiệt độ và độ ẩm theo thời gian
- 📁 **Xuất file CSV** để lưu trữ dữ liệu
- 🔄 **MQTT** kết nối qua broker EMQX công cộng

---

## 🧠 Logic Hoạt Động

| Chế độ | LED1 | LED2 | LED3 |
|--------|------|------|------|
| **Tự động** | Sáng khi nhiệt độ > 30°C | Sáng khi độ ẩm > 70% | Sáng khi trời TỐI |
| **Thủ công** | Bấm nút Bật/Tắt | Bấm nút Bật/Tắt | Bấm nút Bật/Tắt |

---

## 🔌 Sơ Đồ Chân Cắm (Wiring Diagram)

| Linh kiện | Chân ESP8266 | Ghi chú |
|-----------|--------------|---------|
| DHT11 (DATA) | GPIO4 (D2) | Kéo lên 10kΩ |
| LDR | GPIO16 (D0) | Kéo xuống GND qua 10kΩ |
| LED1 (+) | GPIO0 (D3) | Qua 220Ω |
| LED2 (+) | GPIO5 (D1) | Qua 220Ω |
| LED3 (+) | GPIO12 (D6) | Qua 220Ω |
| GND chung | GND | Tất cả LED và cảm biến |

### Hình ảnh mô tả:
<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/02d9fbf9-a4a0-44ce-aeba-9355ff7fefe3" />


---

## 📦 Danh Sách Linh Kiện

| Linh kiện | Số lượng | Giá tham khảo |
|-----------|----------|---------------|
| ESP8266 NodeMCU | 1 | 50.000đ |
| Cảm biến DHT11 | 1 | 35.000đ |
| Cảm biến LDR | 1 | 25.000đ |
| Điện trở 10kΩ | 2 | 1.000đ |
| Điện trở 220Ω | 3 | 1.000đ |
| LED (màu tùy chọn) | 3 | 3.000đ |
| Breadboard | 1 | 30.000đ |
| Dây cắm (jumper) | 10 | 10.000đ |
| **Tổng** | | **~155.000đ** |

---

## 💻 Cài Đặt Phần Mềm

### 1. Cài đặt Arduino IDE

#### Bước 1: Tải Arduino IDE
- Truy cập: https://www.arduino.cc/en/software
- Tải phiên bản phù hợp với hệ điều hành của bạn:
  - **Windows**: chọn Windows Installer
  - **Mac**: chọn macOS Intel hoặc Apple Silicon
  - **Linux**: chọn Linux 64-bit

#### Bước 2: Cài đặt như phần mềm bình thường
- Nhấn **Next → Next → Install** (Windows)
- Kéo vào thư mục **Applications** (Mac)
- `sudo apt install ./arduino-xxx.deb` (Linux)

#### Bước 3: Thêm board ESP8266 vào Arduino IDE

1. Mở **Arduino IDE**
2. Vào **File → Preferences** (Windows) hoặc **Arduino → Settings** (Mac)
3. Trong ô **"Additional Boards Manager URLs"**, thêm đường dẫn:
https://arduino.esp8266.com/stable/package_esp8266com_index.json
4. Nhấn **OK**
5. Vào **Tools → Board → Boards Manager**
6. Tìm kiếm từ khóa **"esp8266"**
7. Cài đặt gói **"esp8266 by ESP8266 Community"** (bản mới nhất)

#### Bước 4: Cài đặt thư viện cần thiết

Vào **Sketch → Include Library → Manage Libraries** (hoặc `Ctrl+Shift+I`)

Tìm và **cài đặt** 2 thư viện sau:

| Tên thư viện | Tác giả | Cách tìm |
|--------------|---------|----------|
| **DHT sensor library** | Adafruit | Gõ "DHT" vào ô tìm kiếm |
| **PubSubClient** | Nick O'Leary | Gõ "PubSubClient" vào ô tìm kiếm |

> ⚠️ **Lưu ý**: Nhấn **Install** cho mỗi thư viện, không cần cài thêm thư viện phụ thuộc.

---

## 📥 Tải Code Dự Án
### Cách tạo file:

1. Tạo thư mục `ESP8266_IoT_Project` trên Desktop
2. Trong thư mục đó, tạo thư mục `ESP8266_Code`
3. Trong thư mục `ESP8266_Code`, tạo file `ESP8266_Code.ino`
4. Tạo thư mục `Web_Interface`
5. Trong thư mục `Web_Interface`, tạo file `index.html`

### Copy code vào từng file:

- **ESP8266_Code.ino**: Copy code Arduino ở [Phần code Arduino](#-code-arduino-ino)
- **index.html**: Copy code HTML ở [Phần code web](#-code-web-indexhtml)

> 💡 **Mẹo**: Bạn có thể tải file mẫu từ GitHub hoặc tự tạo tay.

---

## 📟 Code Arduino (ESP8266_Code.ino)

**Cách sử dụng**: Bôi đen toàn bộ code bên dưới → Copy → Paste vào file `ESP8266_Code.ino`

```cpp

### Cách tạo file:

1. Tạo thư mục `ESP8266_IoT_Project` trên Desktop
2. Trong thư mục đó, tạo thư mục `ESP8266_Code`
3. Trong thư mục `ESP8266_Code`, tạo file `ESP8266_Code.ino`
4. Tạo thư mục `Web_Interface`
5. Trong thư mục `Web_Interface`, tạo file `index.html`

### Copy code vào từng file:

- **ESP8266_Code.ino**: Copy code Arduino ở [Phần code Arduino](#-code-arduino-ino)
- **index.html**: Copy code HTML ở [Phần code web](#-code-web-indexhtml)

> 💡 **Mẹo**: Bạn có thể tải file mẫu từ GitHub hoặc tự tạo tay.

---

## 📟 Code Arduino (ESP8266_Code.ino)

**Cách sử dụng**: Bôi đen toàn bộ code bên dưới → Copy → Paste vào file `ESP8266_Code.ino`

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ========== CẤU HÌNH WiFi & MQTT ==========
const char* ssid = "xx";           // 🔧 THAY ĐỔI: Tên WiFi nhà bạn
const char* password = "xx";   // 🔧 THAY ĐỔI: Mật khẩu WiFi
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
  if (led == 1 && led1State != state) {
    led1State = state;
    digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
    Serial.print(source); Serial.print(" LED1: "); Serial.println(led1State ? "BAT" : "TAT");
    client.publish(topic_led1_status, led1State ? "BAT" : "TAT");
  }
  else if (led == 2 && led2State != state) {
    led2State = state;
    digitalWrite(LED2_PIN, led2State ? HIGH : LOW);
    Serial.print(source); Serial.print(" LED2: "); Serial.println(led2State ? "BAT" : "TAT");
    client.publish(topic_led2_status, led2State ? "BAT" : "TAT");
  }
  else if (led == 3 && led3State != state) {
    led3State = state;
    digitalWrite(LED3_PIN, led3State ? HIGH : LOW);
    Serial.print(source); Serial.print(" LED3: "); Serial.println(led3State ? "BAT" : "TAT");
    client.publish(topic_led3_status, led3State ? "BAT" : "TAT");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) message += (char)payload[i];
  String topicStr = String(topic);
  
  if (topicStr == topic_get_status) { publishFullStatus(); return; }
  
  if (topicStr == topic_mode) {
    if (message == "auto" && !autoMode) {
      autoMode = true;
      Serial.println("=== CHUYEN SANG CHE DO: TU DONG ===");
      client.publish(topic_mode, "auto");
      autoControlLEDs();
    } 
    else if (message == "manual" && autoMode) {
      autoMode = false;
      Serial.println("=== CHUYEN SANG CHE DO: THU CONG ===");
      client.publish(topic_mode, "manual");
    }
    return;
  }
  
  if (!autoMode) {
    if (topicStr == topic_led1) setLED(1, message == "1", "THU CONG");
    else if (topicStr == topic_led2) setLED(2, message == "1", "THU CONG");
    else if (topicStr == topic_led3) setLED(3, message == "1", "THU CONG");
  }
}

void autoControlLEDs() {
  if (!autoMode) return;
  setLED(1, lastTemperature > 30, "TU DONG");
  setLED(2, lastHumidity > 70, "TU DONG");
  setLED(3, lastLdrValue > 500, "TU DONG");
}

void loop() {
  if (!client.connected()) connectMQTT();
  client.loop();
  
  unsigned long now = millis();
  
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;
    
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int ldrValue = analogRead(LDRPIN);
    
    if (!isnan(humidity) && !isnan(temperature)) {
      lastTemperature = temperature;
      lastHumidity = humidity;
      lastLdrValue = ldrValue;
      
      String ldrStatus = (ldrValue > 500) ? "Toi" : "Sang";
      
      char tempStr[10], humStr[10];
      dtostrf(temperature, 4, 1, tempStr);
      dtostrf(humidity, 4, 1, humStr);
      
      client.publish(topic_temp, tempStr);
      client.publish(topic_hum, humStr);
      client.publish(topic_ldr, ldrStatus.c_str());
      
      Serial.print("Nhiet do: "); Serial.print(temperature);
      Serial.print("C | Do am: "); Serial.print(humidity);
      Serial.print("% | LDR: "); Serial.print(ldrValue);
      Serial.print(" ("); Serial.print(ldrStatus);
      Serial.print(") | LED1: "); Serial.print(led1State ? "BAT" : "TAT");
      Serial.print(" | LED2: "); Serial.print(led2State ? "BAT" : "TAT");
      Serial.print(" | LED3: "); Serial.println(led3State ? "BAT" : "TAT");
    }
  }
  
  if (now - lastLEDCheck >= ledCheckInterval) {
    lastLEDCheck = now;
    if (autoMode) autoControlLEDs();
  }
}
```
**## 🖥️ Code Web (index.html)**
```
Cách sử dụng: Bôi đen toàn bộ code bên dưới → Copy → Paste vào file index.html
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP8266 - Điều Khiển Thông Minh</title>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/mqtt/4.3.7/mqtt.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: 'Segoe UI', Roboto, sans-serif; background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%); min-height: 100vh; padding: 10px; }
        .container { max-width: 1200px; margin: 0 auto; }
        .header { text-align: center; margin-bottom: 15px; }
        .header h1 { font-size: 1.4em; color: white; }
        .header p { color: rgba(255,255,255,0.85); font-size: 0.7em; }
        .mode-wrapper { text-align: center; margin-bottom: 15px; }
        .mode-switch { background: white; border-radius: 40px; padding: 4px; display: inline-flex; gap: 4px; }
        .mode-btn { padding: 6px 20px; border: none; border-radius: 40px; font-size: 0.85em; font-weight: 600; cursor: pointer; font-family: inherit; }
        .mode-auto { background: #10b981; color: white; }
        .mode-manual { background: #f59e0b; color: white; }
        .mode-auto.active { background: #059669; }
        .mode-manual.active { background: #d97706; }
        .sensor-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; margin-bottom: 15px; }
        .card { background: white; border-radius: 12px; padding: 10px; box-shadow: 0 4px 12px rgba(0,0,0,0.1); }
        .card-title { font-size: 0.8em; color: #666; margin-bottom: 5px; font-weight: 600; }
        .card-value { font-size: 1.8em; font-weight: 700; color: #1e293b; margin-bottom: 8px; }
        .card-unit { font-size: 0.4em; color: #94a3b8; }
        .led-status-row { display: flex; gap: 8px; margin-bottom: 6px; font-size: 0.7em; align-items: center; }
        .led-badge { display: inline-block; padding: 2px 10px; border-radius: 20px; font-weight: 600; font-size: 0.7em; }
        .led-badge-on { background: #22c55e; color: white; }
        .led-badge-off { background: #9ca3af; color: white; }
        .led-controls { display: flex; gap: 6px; margin: 8px 0 5px; }
        .led-btn { flex: 1; padding: 5px; border: none; border-radius: 6px; font-weight: 600; cursor: pointer; font-size: 0.7em; }
        .led-on { background: #22c55e; color: white; }
        .led-off { background: #ef4444; color: white; }
        .led-auto-note { font-size: 0.6em; color: #64748b; font-style: italic; }
        .data-section { background: white; border-radius: 12px; padding: 10px; margin-bottom: 15px; }
        .data-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; flex-wrap: wrap; }
        .data-header h3 { font-size: 0.85em; }
        .export-btn { background: #3b82f6; color: white; padding: 5px 12px; border: none; border-radius: 6px; cursor: pointer; font-size: 0.7em; font-weight: 600; }
        .table-wrapper { max-height: 140px; overflow-y: auto; border: 1px solid #e2e8f0; border-radius: 8px; }
        table { width: 100%; border-collapse: collapse; font-size: 0.65em; }
        th, td { padding: 5px 4px; text-align: left; border-bottom: 1px solid #e2e8f0; }
        th { background: #f8fafc; position: sticky; top: 0; font-weight: 600; }
        .charts-container { display: grid; grid-template-columns: repeat(2, 1fr); gap: 10px; }
        .chart-card { background: white; border-radius: 12px; padding: 8px; }
        .chart-card h3 { font-size: 0.75em; text-align: center; margin-bottom: 5px; }
        .chart-wrapper { position: relative; height: 170px; width: 100%; }
        canvas { max-height: 170px !important; width: 100% !important; }
        @media (max-width: 700px) {
            .sensor-grid { grid-template-columns: 1fr; }
            .charts-container { grid-template-columns: 1fr; }
            .chart-wrapper { height: 150px; }
        }
    </style>
</head>
<body>
<div class="container">
    <div class="header">
        <h1>ESP8266 - Điều Khiển Thông Minh</h1>
        <p>Nhiệt độ | Độ ẩm | Ánh sáng | Điều khiển LED</p>
    </div>
    <div class="mode-wrapper">
        <div class="mode-switch">
            <button id="autoModeBtn" class="mode-btn mode-auto active">Tự động</button>
            <button id="manualModeBtn" class="mode-btn mode-manual">Thủ công</button>
        </div>
    </div>
    <div class="sensor-grid">
        <div class="card">
            <div class="card-title">Nhiệt độ</div>
            <div class="card-value"><span id="tempValue">--</span><span class="card-unit">°C</span></div>
            <div class="led-status-row"><span>LED1:</span><span id="led1Badge" class="led-badge led-badge-off">TẮT</span></div>
            <div class="led-controls">
                <button class="led-btn led-on" onclick="controlLED('led1', '1')">Bật LED1</button>
                <button class="led-btn led-off" onclick="controlLED('led1', '0')">Tắt LED1</button>
            </div>
            <div class="led-auto-note" id="led1Note">Tự động: sáng khi nhiệt độ > 30°C</div>
        </div>
        <div class="card">
            <div class="card-title">Độ ẩm</div>
            <div class="card-value"><span id="humValue">--</span><span class="card-unit">%</span></div>
            <div class="led-status-row"><span>LED2:</span><span id="led2Badge" class="led-badge led-badge-off">TẮT</span></div>
            <div class="led-controls">
                <button class="led-btn led-on" onclick="controlLED('led2', '1')">Bật LED2</button>
                <button class="led-btn led-off" onclick="controlLED('led2', '0')">Tắt LED2</button>
            </div>
            <div class="led-auto-note" id="led2Note">Tự động: sáng khi độ ẩm > 70%</div>
        </div>
        <div class="card">
            <div class="card-title">Ánh sáng (LDR)</div>
            <div class="card-value"><span id="ldrValue">--</span></div>
            <div class="led-status-row"><span>LED3:</span><span id="led3Badge" class="led-badge led-badge-off">TẮT</span></div>
            <div class="led-controls">
                <button class="led-btn led-on" onclick="controlLED('led3', '1')">Bật LED3</button>
                <button class="led-btn led-off" onclick="controlLED('led3', '0')">Tắt LED3</button>
            </div>
            <div class="led-auto-note" id="led3Note">Tự động: sáng khi trời tối</div>
        </div>
    </div>
    <div class="data-section">
        <div class="data-header">
            <h3>Dữ liệu thời gian thực</h3>
            <button class="export-btn" onclick="exportCSV()">Xuất file CSV</button>
        </div>
        <div class="table-wrapper">
            <table id="dataTable">
                <thead><tr><th>Thời gian</th><th>Nhiệt độ</th><th>Độ ẩm</th><th>LDR</th><th>LED1</th><th>LED2</th><th>LED3</th></tr></thead>
                <tbody></tbody>
             </table>
        </div>
    </div>
    <div class="charts-container">
        <div class="chart-card"><h3>Biểu đồ nhiệt độ</h3><div class="chart-wrapper"><canvas id="tempChart"></canvas></div></div>
        <div class="chart-card"><h3>Biểu đồ độ ẩm</h3><div class="chart-wrapper"><canvas id="humChart"></canvas></div></div>
    </div>
</div>
<script>
    const broker = "wss://broker.emqx.io:8084/mqtt";
    const client = mqtt.connect(broker);
    let tempChart, humChart;
    let timeData = [], tempHistory = [], humHistory = [], csvRows = [];
    let led1State = null, led2State = null, led3State = null;
    
    function updateLEDBadges() {
        const l1 = document.getElementById("led1Badge"), l2 = document.getElementById("led2Badge"), l3 = document.getElementById("led3Badge");
        if (led1State === 1) { l1.innerHTML = "BẬT"; l1.className = "led-badge led-badge-on"; }
        else if (led1State === 0) { l1.innerHTML = "TẮT"; l1.className = "led-badge led-badge-off"; }
        if (led2State === 1) { l2.innerHTML = "BẬT"; l2.className = "led-badge led-badge-on"; }
        else if (led2State === 0) { l2.innerHTML = "TẮT"; l2.className = "led-badge led-badge-off"; }
        if (led3State === 1) { l3.innerHTML = "BẬT"; l3.className = "led-badge led-badge-on"; }
        else if (led3State === 0) { l3.innerHTML = "TẮT"; l3.className = "led-badge led-badge-off"; }
    }
    
    function controlLED(led, state) {
        let topic = led === "led1" ? "esp8266/led1/control" : (led === "led2" ? "esp8266/led2/control" : "esp8266/led3/control");
        client.publish(topic, state);
    }
    
    function initCharts() {
        tempChart = new Chart(document.getElementById('tempChart'), { type: 'line', data: { labels: timeData, datasets: [{ label: 'Nhiệt độ (°C)', data: tempHistory, borderColor: '#ef4444', tension: 0.3, fill: true, pointRadius: 2 }] }, options: { responsive: true, maintainAspectRatio: true } });
        humChart = new Chart(document.getElementById('humChart'), { type: 'line', data: { labels: timeData, datasets: [{ label: 'Độ ẩm (%)', data: humHistory, borderColor: '#3b82f6', tension: 0.3, fill: true, pointRadius: 2 }] }, options: { responsive: true, maintainAspectRatio: true } });
    }
    
    function updateCharts(time, temp, hum) {
        timeData.push(time); tempHistory.push(temp); humHistory.push(hum);
        if (timeData.length > 20) { timeData.shift(); tempHistory.shift(); humHistory.shift(); }
        tempChart.update(); humChart.update();
    }
    
    function addTableRow(time, temp, hum, ldr, led1, led2, led3) {
        const tbody = document.getElementById('dataTable').querySelector('tbody');
        const row = document.createElement('tr');
        row.innerHTML = `<td>${time}</td><td>${temp}</td><td>${hum}</td><td>${ldr}</td><td>${led1}</td><td>${led2}</td><td>${led3}</td>`;
        tbody.appendChild(row);
        if (tbody.children.length > 12) tbody.deleteRow(0);
        csvRows.push({ time, temp, hum, ldr, led1, led2, led3 });
        if (csvRows.length > 100) csvRows.shift();
    }
    
    function exportCSV() {
        let content = "Thời gian,Nhiệt độ (°C),Độ ẩm (%),LDR,LED1,LED2,LED3\n";
        csvRows.forEach(r => content += `${r.time},${r.temp},${r.hum},${r.ldr},${r.led1},${r.led2},${r.led3}\n`);
        const blob = new Blob(["\uFEFF" + content], { type: "text/csv;charset=utf-8;" });
        const link = document.createElement("a");
        link.href = URL.createObjectURL(blob);
        link.download = `dulieu_${new Date().toISOString().slice(0,19)}.csv`;
        link.click();
        URL.revokeObjectURL(link.href);
    }
    
    client.on('connect', () => {
        client.subscribe("esp8266/led1/status"); client.subscribe("esp8266/led2/status"); client.subscribe("esp8266/led3/status");
        client.subscribe("esp8266/mode"); client.subscribe("esp8266/dht11/temperature"); client.subscribe("esp8266/dht11/humidity");
        client.subscribe("esp8266/ldr/status");
        initCharts();
        setTimeout(() => client.publish("esp8266/get/status", "request"), 500);
    });
    
    client.on('message', (topic, message) => {
        const val = message.toString();
        const now = new Date().toLocaleTimeString('vi-VN');
        if (topic === "esp8266/led1/status") { led1State = val === "BAT" ? 1 : 0; updateLEDBadges(); }
        else if (topic === "esp8266/led2/status") { led2State = val === "BAT" ? 1 : 0; updateLEDBadges(); }
        else if (topic === "esp8266/led3/status") { led3State = val === "BAT" ? 1 : 0; updateLEDBadges(); }
        else if (topic === "esp8266/mode") {
            if (val === "auto") {
                document.getElementById("autoModeBtn").classList.add("active");
                document.getElementById("manualModeBtn").classList.remove("active");
                document.getElementById("led1Note").innerHTML = "Tự động: sáng khi nhiệt độ > 30°C";
                document.getElementById("led2Note").innerHTML = "Tự động: sáng khi độ ẩm > 70%";
                document.getElementById("led3Note").innerHTML = "Tự động: sáng khi trời tối";
            } else {
                document.getElementById("manualModeBtn").classList.add("active");
                document.getElementById("autoModeBtn").classList.remove("active");
                document.getElementById("led1Note").innerHTML = "Thủ công: bấm nút để điều khiển";
                document.getElementById("led2Note").innerHTML = "Thủ công: bấm nút để điều khiển";
                document.getElementById("led3Note").innerHTML = "Thủ công: bấm nút để điều khiển";
            }
        }
        else if (topic === "esp8266/dht11/temperature") document.getElementById("tempValue").innerText = val;
        else if (topic === "esp8266/dht11/humidity") document.getElementById("humValue").innerText = val;
        else if (topic === "esp8266/ldr/status") document.getElementById("ldrValue").innerText = val;
        
        const temp = document.getElementById("tempValue").innerText;
        const hum = document.getElementById("humValue").innerText;
        const ldr = document.getElementById("ldrValue").innerText;
        const led1 = led1State === 1 ? "BẬT" : "TẮT";
        const led2 = led2State === 1 ? "BẬT" : "TẮT";
        const led3 = led3State === 1 ? "BẬT" : "TẮT";
        if ((topic === "esp8266/dht11/temperature" || topic === "esp8266/dht11/humidity") && temp !== "--" && hum !== "--") {
            updateCharts(now, parseFloat(temp), parseFloat(hum));
            addTableRow(now, temp, hum, ldr, led1, led2, led3);
        }
    });
    
    document.getElementById("autoModeBtn").onclick = () => client.publish("esp8266/mode", "auto");
    document.getElementById("manualModeBtn").onclick = () => client.publish("esp8266/mode", "manual");
</script>
</body>
</html>
```

## 🚀 Hướng Dẫn Nạp Code Vào ESP8266
```
Bước 1: Kết nối ESP8266 với máy tính
Cắm cáp USB vào ESP8266 và máy tính. Đèn LED trên board sẽ sáng.

Bước 2: Mở code trong Arduino IDE
Mở Arduino IDE

Vào File → Open → Chọn file ESP8266_Code.ino

Bước 3: Chọn Board
Vào Tools → Board → ESP8266 Boards → NodeMCU 1.0 (ESP-12E Module)

Bước 4: Chọn Cổng COM
Vào Tools → Port → Chọn cổng hiển thị (VD: COM3, COM5, hoặc /dev/cu.usbserial-xxx)

💡 Mẹo: Nếu không thấy cổng nào, bạn cần cài driver CH340:

Windows: Tải tại https://www.wch.cn/download/CH341SER_EXE.html

Mac: Tự động nhận diện

Linux: sudo modprobe ch341

Bước 5: Chọn tốc độ Upload
Vào Tools → Upload Speed → Chọn 115200

Bước 6: Nạp code
Nhấn vào nút → (Upload) hoặc nhấn Ctrl + U

Chờ quá trình Compile và Upload:

text
Compiling sketch...
Uploading...
Done uploading.
Bước 7: Mở Serial Monitor
Nhấn Tools → Serial Monitor (hoặc Ctrl + Shift + M)

Chọn 115200 baud ở góc dưới cùng bên phải.

Bạn sẽ thấy dữ liệu hiển thị:

text
Dang ket noi WiFi.....
WiFi da ket noi!
IP: 192.168.1.xxx
MQTT da ket noi!
Nhiet do: 30.5C | Do am: 65% | LDR: 512 (Sang) | LED1: TAT | LED2: TAT | LED3: TAT
✅ Thành công nếu thấy dòng chữ "WiFi da ket noi!" và có địa chỉ IP.
```
## 🌐 Hướng Dẫn Chạy Giao Diện Web
```
Cách 1: Mở trực tiếp bằng trình duyệt (Dễ nhất)
Mở File Explorer (Windows) hoặc Finder (Mac)

Tìm đến thư mục chứa file index.html

Double-click vào file index.html

Trang web sẽ mở trong trình duyệt mặc định

Cách 2: Kéo thả vào trình duyệt
Mở Chrome / Edge / Firefox

Kéo file index.html từ thư mục vào cửa sổ trình duyệt

Cách 3: Dùng Live Server (VSCode - Nâng cao)
Mở Visual Studio Code

Cài extension Live Server

Mở thư mục chứa index.html

Chuột phải vào file → Open with Live Server
```

## Kiểm tra kết nối:
Sau khi mở web, bạn sẽ thấy:

Dòng chữ "MQTT đã kết nối" trong Console (F12)

Dữ liệu nhiệt độ, độ ẩm hiển thị

Biểu đồ bắt đầu vẽ

⚠️ Nếu không thấy dữ liệu: Đợi 5-10 giây hoặc F5 lại trang.

## 📖 Cách Sử Dụng Giao Diện
Màn hình chính:
<<<<<<< HEAD
<img width="736" height="744" alt="image" src="https://github.com/user-attachments/assets/c7709403-3f8a-4809-bced-169944acef14" />
=======
<img width="736" height="744" alt="image" src="https://github.com/user-attachments/assets/c7709403-3f8a-4809-bced-169944acef14" />
## Hệ thống 

<img width="2400" height="1080" alt="image" src="https://github.com/user-attachments/assets/8281c5c5-1bad-4977-8c0a-91080611a239" />

<img width="2869" height="1537" alt="image" src="https://github.com/user-attachments/assets/e399ad09-8308-413c-9cf5-81a6175fe052" />

## Quét mã để điều khiển qua điện thoại
<img width="709" height="690" alt="image" src="https://github.com/user-attachments/assets/b4182764-1d6f-4896-9b36-863232f0dca1" />

