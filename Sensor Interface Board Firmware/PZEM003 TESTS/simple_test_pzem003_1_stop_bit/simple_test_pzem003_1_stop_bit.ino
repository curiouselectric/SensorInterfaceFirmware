#include <Arduino.h>
#include <SoftwareSerial.h>

#define RS485_PWR_PIN 4  // This is a power output for the RS485 port
#define RS485_TX_PIN 3   // This is a Tx (out) for the RS485 port
#define RS485_RX_PIN 2   // This is a Rx (in) for the RS485 port
#define sensorBaudRate 9600

const byte request_dc_v[] = { 0x01, 0x04, 0x00, 0x00, 0x00, 0x01 };       // Read voltage (2 byte) from address 0x0000. 1 LSB = 0.01V (no CRC - that is calculated)
const byte request_dc_i[] = { 0x01, 0x04, 0x00, 0x01, 0x00, 0x01 };       // Read current (2 byte) from address 0x001. 1LSB = 0.01A (no CRC - that is calculated)
const byte request_dc_power[] = { 0x01, 0x04, 0x00, 0x02, 0x00, 0x02 };   // Read power from address 0x0002 (low bits) and 0x0003 (high bits)  1LSB = 0.1W (no CRC - that is calculated)
const byte request_dc_energy[] = { 0x01, 0x04, 0x00, 0x04, 0x00, 0x02 };  // Read energy from address 0x0004 (low bits) and 0x0005 (high bits)  1LSB = 1Wh (no CRC - that is calculated)

byte pzem003RequestNoCRC[6];
byte pzem003Request[8];
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
  mod.setTimeout(500);        // Set timeout to 500mS
}

void loop() {

  float returnedValue;
  // Request the voltage:
  createRequest(request_dc_v, sizeof(request_dc_v));
  mod.write(pzem003Request, sizeof(pzem003Request));
  delay(100);  // Wait a bit for a response...
  returnedValue = getRequestData('v');
  Serial.print("V (V):\t");
  Serial.println(returnedValue);

  // Request the current:
  createRequest(request_dc_i, sizeof(request_dc_i));
  mod.write(pzem003Request, sizeof(pzem003Request));
  delay(100);  // Wait a bit for a response...
  returnedValue = getRequestData('i');

  Serial.print("I (A):\t");
  Serial.println(returnedValue);

  // Request the power:
  createRequest(request_dc_power, sizeof(request_dc_power));
  mod.write(pzem003Request, sizeof(pzem003Request));
  delay(100);  // Wait a bit for a response...
  returnedValue = getRequestData('p');
  Serial.print("P (W):\t");
  Serial.println(returnedValue);

  // Request the energy:
  createRequest(request_dc_energy, sizeof(request_dc_energy));
  mod.write(pzem003Request, sizeof(pzem003Request));
  delay(100);  // Wait a bit for a response...
  returnedValue = getRequestData('e');
  Serial.print("E (Wh):\t");
  Serial.println(returnedValue);

  delay(3000);  // Wait and then repeat
}

void createRequest(byte buf[], int len) {
  // Calculate the CRC16 value (depends upon the sensor ID)
  unsigned int CRCvalue = calc_CRC16(buf, len);
  for (int i = 0; i < len; i++) {
    pzem003Request[i] = buf[i];
  }
  pzem003Request[len] = highByte(CRCvalue);
  pzem003Request[len + 1] = lowByte(CRCvalue);

#ifdef SS_DEBUG
  // Debug output
  Serial.print("REQ: ");
  for (auto x : pzem003Request)  // for each element 'x' in the array 'pzem003Request'
  {
    Serial.print(x, HEX);
    Serial.print(":");
  }
  Serial.println();
//End of debug
#endif
}

float getRequestData(char dataType) {
  // Read the response from the sensor
  byte index = 0;
  while (mod.available()) {
    pzem003Response[index] = mod.read();
    if (pzem003Response[index] == '\n') {
      // got end of line character so break out
      break;
    }
#ifdef SS_DEBUG
    // Debug:
    // Show the data recieved:
    Serial.print(pzem003Response[index], HEX);  // Print the received byte in HEX format
    Serial.print(" ");
// End of debug
#endif
    index++;
  }
#ifdef SS_DEBUG
  // Debug
  Serial.println();
#endif

  switch (dataType) {
    case 'v':
      //Parse and calculate the voltage data:
      return (int(pzem003Response[3] << 8 | pzem003Response[4]) / 100.0);
      break;
    case 'i':
      //Parse and calculate the current data:
      return (int(pzem003Response[3] << 8 | pzem003Response[4]) / 100.0);
      break;
    case 'p':
      //Parse and calculate the power data:
      long int dcPower_Int = 0;
      dcPower_Int = dcPower_Int + long(pzem003Response[3]);
      dcPower_Int << 8;
      dcPower_Int = dcPower_Int + long(pzem003Response[4]);
      dcPower_Int << 8;
      dcPower_Int = dcPower_Int + long(pzem003Response[5]);
      dcPower_Int << 8;
      dcPower_Int = dcPower_Int + long(pzem003Response[6]);
      return ((float)dcPower_Int / 10.0);
      break;
    case 'e':
      //Parse and calculate the energy data:
      long int dcEnergy_Int = 0;
      dcEnergy_Int = dcEnergy_Int + long(pzem003Response[3]);
      dcEnergy_Int << 8;
      dcEnergy_Int = dcEnergy_Int + long(pzem003Response[4]);
      dcEnergy_Int << 8;
      dcEnergy_Int = dcEnergy_Int + long(pzem003Response[5]);
      dcEnergy_Int << 8;
      dcEnergy_Int = dcEnergy_Int + long(pzem003Response[6]);
      return ((float)dcEnergy_Int);
      break;
  }
}

unsigned int calc_CRC16(unsigned char *buf, int len) {
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++) {
    crc ^= (unsigned int)buf[pos];
    for (int i = 8; i != 0; i--) {
      if ((crc & 0x0001) != 0) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
  return crc;
}
