#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

WebServer server(80);

VL53L0X sensor;
int currentDistance = 0;

void sentWebPage(){
  File file = LittleFS.open ("/index.html", "r");
  server.streamFile(file, "text/html");
  file.close();
}

void sentDistance(){
  server.send(200, "text/plain", String(currentDistance));
}

void setup(){
  Serial.begin(115200);
  Wire.begin(4, 5);

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS error");
    return;
  } else {
    Serial.println("LittleFS OK!");
  }

  sensor.setTimeout(500);
  if(!sensor.init()){
    Serial.println("Fail to conect VL53LOX");
    while (1);
  }

  sensor.startContinuous();

  Serial.println("Initializing Wi-Fi...");
  WiFi.softAP("Theremin-ESP", "");

  Serial.print("Connect to the 'Theremin-ESP' Wi-Fi network and access the IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", sentWebPage);
  server.on("/distance", sentDistance);

  server.begin();
}

void loop(){
  currentDistance = sensor.readRangeContinuousMillimeters();
  Serial.print(currentDistance);
  Serial.println(" mm");
  server.handleClient();
}
