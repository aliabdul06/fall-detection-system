#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <PubSubClient.h>

// ----- CHANGE THESE -----
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";
const char* mqtt_server = "BROKER_IP";
// ------------------------

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MPU6050 mpu;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  mpu.initialize();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("MQTT Connected!");
    } else {
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float Ax = ax / 16384.0;
  float Ay = ay / 16384.0;
  float Az = az / 16384.0;
  float A = sqrt(Ax*Ax + Ay*Ay + Az*Az);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Accel: ");
  display.print(A);
  display.println(" g");

  if (A > 2.5) {
    display.setTextSize(2);
    display.setCursor(0, 30);
    display.println("FALL !");
    client.publish("fall/detection", "ALERT: FALL");
    Serial.println("FALL DETECTED");
  } else {
    display.setTextSize(2);
    display.setCursor(0, 30);
    display.println("OK");
    Serial.println("OK");
  }

  display.display();
  delay(200);
}