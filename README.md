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
