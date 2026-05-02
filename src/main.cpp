#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
int currentDistance = 0;

void setup(){
  Serial.begin(115200);
  Wire.begin(4, 5);

  sensor.setTimeout(500);
  if(!sensor.init()){
    Serial.println("Fail to conect VL53LOX");
    while (1);
  }

  sensor.startContinuous();
}

void loop(){
  currentDistance = sensor.readRangeContinuousMillimeters();
  Serial.print(currentDistance);
  Serial.println(" mm");
}
