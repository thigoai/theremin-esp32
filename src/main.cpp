#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

WebServer server(80);
VL53L0X sensor;
int currentDistance = 0;

void sendWebPage() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(404, "text/plain", "index.html not found");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

void sendDistance() {
  server.send(200, "text/plain", String(currentDistance));
}

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5);

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS error");
    return;
  } else {
    Serial.println("LittleFS OK!");
  }

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to connect VL53L0X");
    while (1);
  }
  sensor.startContinuous();

  Serial.println("Initializing Wi-Fi...");
  WiFi.softAP("Theremin-ESP", "");
  Serial.print("Connect to 'Theremin-ESP' and access: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", sendWebPage);
  server.on("/distance", sendDistance);

  server.on("/PressStart2P.ttf", []() {
    File file = LittleFS.open("/PressStart2P.ttf", "r");
    if (!file) {
      server.send(404, "text/plain", "Font not found");
      return;
    }
    server.streamFile(file, "font/truetype");
    file.close();
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  currentDistance = sensor.readRangeContinuousMillimeters();
  Serial.print(currentDistance);
  Serial.println(" mm");
  server.handleClient();
}