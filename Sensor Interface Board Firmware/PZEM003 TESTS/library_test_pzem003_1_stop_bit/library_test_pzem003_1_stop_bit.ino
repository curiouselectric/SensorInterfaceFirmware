
#define PZEM_003_RS485_SENSOR

#include <Arduino.h>
#include "pzem_003_rs485.h"


void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  myPzem003Sensor.begin();
}

void loop() {


  myPzem003Sensor.pwr_on();
  if (myPzem003Sensor.readVoltage(1)) {
    Serial.print("V (V):\t");
    Serial.println(myPzem003Sensor.dcVoltage);
  } else {
    Serial.println("V (V): ERR");
  }

  if (myPzem003Sensor.readCurrent(1)) {
    Serial.print("I (A):\t");
    Serial.println(myPzem003Sensor.dcCurrent);
  } else {
    Serial.println("I (A): ERR");
  }

  if (myPzem003Sensor.readPower(1)) {
    Serial.print("P (W):\t");
    Serial.println(myPzem003Sensor.dcPower);
  } else {
    Serial.println("P (W): ERR");
  }

  if (myPzem003Sensor.readEnergy(1)) {
    Serial.print("E (Wh):\t");
    Serial.println(myPzem003Sensor.dcEnergy);
  } else {
    Serial.println("E (Wh): ERR");
  }

  if (myPzem003Sensor.readAll(1)) {
    Serial.print("V (V):\t");
    Serial.print(myPzem003Sensor.dcVoltage);
    Serial.print("\t I (A):\t");
    Serial.print(myPzem003Sensor.dcCurrent);
    Serial.print("\t P (W):\t");
    Serial.print(myPzem003Sensor.dcPower);
    Serial.print("\t E (Wh):\t");
    Serial.println(myPzem003Sensor.dcEnergy);
  } else {
    Serial.println("E (Wh): ERR");
  }
  myPzem003Sensor.pwr_off();

  delay(3000);  // Wait and then repeat
}
