#include <Arduino.h>
#include <SoftwareSerial.h>

#define RS485_PWR_PIN 4  // This is a power output for the RS485 port
#define RS485_TX_PIN 3   // This is a Tx (out) for the RS485 port
#define RS485_RX_PIN 2   // This is a Rx (in) for the RS485 port
#define sensorBaudRate 9600

const byte request_dc_v_1[] = { 0x01, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xCA };  // Read voltage from address 0x01
const byte request_dc_v_2[] = { 0x02, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xF9 };  // Read voltage from address 0x02

byte pzem003Response[9];  // a byte array to hold incoming data


SoftwareSerial mod(RS485_RX_PIN, RS485_TX_PIN);  // Software serial for RS485 communication

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  // Set up the RS485 power output
  pinMode(RS485_PWR_PIN, OUTPUT);
  digitalWrite(RS485_PWR_PIN, HIGH);

  // Set the data rate for the SoftwareSerial port
  // For peacefair equipment: 8 bit, No parity, 1 Stop bits
  /* Start Serial */
  mod.begin(sensorBaudRate);  // Initialize software serial communication at 9600 baud rate
}

void loop() {
  // Send the request to the soil sensor
  mod.write(request_dc_v_1, sizeof(request_dc_v_1));
  delay(200);

  // Wait for the response from the sensor or timeout after 500 msecond
  unsigned long startTime = millis();

  while (mod.available() < 6 && millis() - startTime < 500) {
    delay(1);
  }

  if (mod.available() >= 6)  // If valid response receivedPulse sensor modules
  {
    // Read the response from the sensor
    byte index = 0;
    while (mod.available() && index < 9) {
      pzem003Response[index] = mod.read();

      // Show the data recieved:
      Serial.print(pzem003Response[index], HEX);  // Print the received byte in HEX format
      Serial.print(" ");
      index++;
    }
  }
  Serial.println();

  //Parse and calculate the voltage data:
  int dcVoltage_Int = int(pzem003Response[4] << 8 | pzem003Response[5]);
  float dcVoltage = dcVoltage_Int / 10.0;
  Serial.print("Volatge (mV):/t");
  Serial.print(dcVoltage_Int);
  Serial.print("/tVolatge (V):/t");
  Serial.println(dcVoltage);  
  delay(3000);  // Wait and then repeat
}
