#include "config.h"

#ifdef PZEM_003_RS485_SENSOR
#include <SoftwareSerial.h>

SoftwareSerial mod(RS485_RX_PIN, RS485_TX_PIN);  // Software serial for RS485 communication

pzem003Sensor myPzem003Sensor;

/****************************************************************************/
/***       Local Variable                                                 ***/
/****************************************************************************/
//#define sensorBaudRate 4800
#define sensorBaudRate 9600

// //With Module ID of 0x01:
// const byte request_dc_v[] = { 0x01, 0x04, 0x00, 0x00, 0x00, 0x01 };       // Read voltage (2 byte) from address 0x0000. 1 LSB = 0.01V (no CRC - that is calculated)
// const byte request_dc_i[] = { 0x01, 0x04, 0x00, 0x01, 0x00, 0x01 };       // Read current (2 byte) from address 0x001. 1LSB = 0.01A (no CRC - that is calculated)
// const byte request_dc_power[] = { 0x01, 0x04, 0x00, 0x02, 0x00, 0x02 };   // Read power from address 0x0002 (low bits) and 0x0003 (high bits)  1LSB = 0.1W (no CRC - that is calculated)
// const byte request_dc_energy[] = { 0x01, 0x04, 0x00, 0x04, 0x00, 0x02 };  // Read energy from address 0x0004 (low bits) and 0x0005 (high bits)  1LSB = 1Wh (no CRC - that is calculated)

//Without module ID (we add this when creating the request:)
const byte request_dc_v[] = { 0x04, 0x00, 0x00, 0x00, 0x01 };       // Read voltage (2 byte) from address 0x0000. 1 LSB = 0.01V (no CRC - that is calculated)
const byte request_dc_i[] = { 0x04, 0x00, 0x01, 0x00, 0x01 };       // Read current (2 byte) from address 0x001. 1LSB = 0.01A (no CRC - that is calculated)
const byte request_dc_power[] = { 0x04, 0x00, 0x02, 0x00, 0x02 };   // Read power from address 0x0002 (low bits) and 0x0003 (high bits)  1LSB = 0.1W (no CRC - that is calculated)
const byte request_dc_energy[] = { 0x04, 0x00, 0x04, 0x00, 0x02 };  // Read energy from address 0x0004 (low bits) and 0x0005 (high bits)  1LSB = 1Wh (no CRC - that is calculated)
// const byte request_dc_all[] = { 0x04, 0x00, 0x00, 0x00, 0x06 };  // Read all data from address 0x0000. 1 LSB = 0.01V (no CRC - that is calculated)

byte pzem003RequestNoCRC[6];
byte pzem003Request[8];
byte pzem003Response[16];  // a byte array to hold incoming data

/****************************************************************************/
/***       Class member Functions                                         ***/
/****************************************************************************/

void pzem003Sensor::begin(void) {

  // Set up the RS485 power output
  pwr_on();
  // Set the data rate for the SoftwareSerial port
  // For peacefair equipment: 8 bit, No parity, 1 Stop bits
  /* Start Serial */
  mod.begin(sensorBaudRate);  // Initialize software serial communication at 9600 baud rate
  mod.setTimeout(500);        // Set timeout to 500mS
  pwr_off();
}

bool pzem003Sensor::readVoltage(byte ID) {
  // Add the module ID to the request
  pzem003RequestNoCRC[0] = ID;
  for (int i = 0; i < sizeof(request_dc_v); i++) {
    pzem003RequestNoCRC[i + 1] = request_dc_v[i];
  }
  // Request the voltage:
  createRequest(pzem003RequestNoCRC, sizeof(pzem003RequestNoCRC));
  mod.write(pzem003Request, sizeof(pzem003Request));
  delay(200);  // Wait a bit for a response...
  if (getRequestData('v') == true) {
    return (true);
  } else {
    return (false);
  }
}

bool pzem003Sensor::readCurrent(byte ID) {
  // Add the module ID to the request
  pzem003RequestNoCRC[0] = ID;
  for (int i = 0; i < sizeof(request_dc_i); i++) {
    pzem003RequestNoCRC[i + 1] = request_dc_i[i];
  }
  // Request the voltage:
  createRequest(pzem003RequestNoCRC, sizeof(pzem003RequestNoCRC));
  mod.write(pzem003Request, sizeof(pzem003Request));
  delay(100);  // Wait a bit for a response...
  if (getRequestData('i') == true) {
    return (true);
  } else {
    return (false);
  }
}

bool pzem003Sensor::readPower(byte ID) {
  // Add the module ID to the request
  pzem003RequestNoCRC[0] = ID;
  for (int i = 0; i < sizeof(request_dc_power); i++) {
    pzem003RequestNoCRC[i + 1] = request_dc_power[i];
  }
  // Request the voltage:
  createRequest(pzem003RequestNoCRC, sizeof(pzem003RequestNoCRC));
  mod.write(pzem003Request, sizeof(pzem003Request));
  delay(100);  // Wait a bit for a response...
  if (getRequestData('p') == true) {
    return (true);
  } else {
    return (false);
  }
}

bool pzem003Sensor::readEnergy(byte ID) {
  // Add the module ID to the request
  pzem003RequestNoCRC[0] = ID;
  for (int i = 0; i < sizeof(request_dc_energy); i++) {
    pzem003RequestNoCRC[i + 1] = request_dc_energy[i];
  }
  // Request the voltage:
  createRequest(pzem003RequestNoCRC, sizeof(pzem003RequestNoCRC));
  mod.write(pzem003Request, sizeof(pzem003Request));
  delay(100);  // Wait a bit for a response...
  if (getRequestData('e') == true) {
    return (true);
  } else {
    return (false);
  }
}

// bool pzem003Sensor::readAll(byte ID) {
//   // Add the module ID to the request
//   pzem003RequestNoCRC[0] = ID;
//   for (int i = 0; i < sizeof(request_dc_all); i++) {
//     pzem003RequestNoCRC[i + 1] = request_dc_all[i];
//   }
//   // Request the voltage:
//   createRequest(pzem003RequestNoCRC, sizeof(pzem003RequestNoCRC));
//   mod.write(pzem003Request, sizeof(pzem003Request));
//   delay(100);  // Wait a bit for a response...
//   if (getRequestData('a') == true) {
//     return (true);
//   } else {
//     return (false);
//   }
// }

void pzem003Sensor::createRequest(byte buf[], int len) {
  // Calculate the CRC16 value (depends upon the sensor ID)
  unsigned int CRCvalue = calc_CRC16(buf, len);
  for (int i = 0; i < len; i++) {
    pzem003Request[i] = buf[i];
  }
  pzem003Request[len] = highByte(CRCvalue);
  pzem003Request[len + 1] = lowByte(CRCvalue);

#ifdef DEBUG_PZEM003
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

bool pzem003Sensor::getRequestData(char dataType) {
  // Read the response from the sensor
  byte index = 0;
  while (mod.available()) {
    pzem003Response[index] = mod.read();
    if (pzem003Response[index] == '\n') {
      // got end of line character so break out
      break;
    }
#ifdef DEBUG_PZEM003
    // Debug:
    // Show the data recieved:
    Serial.print(pzem003Response[index], HEX);  // Print the received byte in HEX format
    Serial.print(" ");
// End of debug
#endif
    index++;
  }
#ifdef DEBUG_PZEM003
  // Debug
  Serial.println();
#endif

  // If no data returned (after timeout delay of 500ms) then return an error
  if (index <= 5) {
    //Rough test that we did not get enough bytes back.
    return false;
  }

  // Deal with the returned data
  switch ((char)dataType) {
    case (char)'v':
      //Parse and calculate the voltage data:
      dcVoltage = (int(pzem003Response[3] << 8 | pzem003Response[4]) / 100.0);
      break;
    case (char)'i':
      //Parse and calculate the current data:
      dcCurrent = (int(pzem003Response[3] << 8 | pzem003Response[4]) / 100.0);
      break;
    case (char)'p':
      //Parse and calculate the power data:
      long int dcPower_Int = 0;
      dcPower_Int = dcPower_Int + long(pzem003Response[3]);
      dcPower_Int << 8;
      dcPower_Int = dcPower_Int + long(pzem003Response[4]);
      dcPower_Int << 8;
      dcPower_Int = dcPower_Int + long(pzem003Response[5]);
      dcPower_Int << 8;
      dcPower_Int = dcPower_Int + long(pzem003Response[6]);
      dcPower = ((float)dcPower_Int / 10.0);
      break;
    case (char)'e':
      //Parse and calculate the energy data:
      long int dcEnergy_Int = 0;
      dcEnergy_Int = dcEnergy_Int + long(pzem003Response[3]);
      dcEnergy_Int << 8;
      dcEnergy_Int = dcEnergy_Int + long(pzem003Response[4]);
      dcEnergy_Int << 8;
      dcEnergy_Int = dcEnergy_Int + long(pzem003Response[5]);
      dcEnergy_Int << 8;
      dcEnergy_Int = dcEnergy_Int + long(pzem003Response[6]);
      dcEnergy = ((float)dcEnergy_Int);
      break;
//     case (char)'a':
//       dcVoltage = (int(pzem003Response[3] << 8 | pzem003Response[4]) / 100.0);
//       dcCurrent = (int(pzem003Response[5] << 8 | pzem003Response[6]) / 100.0);
//       long int dc_Int = 0;
//       dc_Int = dc_Int + long(pzem003Response[7]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzem003Response[8]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzem003Response[9]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzem003Response[10]);
//       dcPower = ((float)dc_Int / 10.0);
//       dc_Int = 0;
//       dc_Int = dc_Int + long(pzem003Response[11]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzem003Response[12]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzem003Response[13]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzem003Response[14]);
//       dcEnergy = ((float)dc_Int);
// #ifdef DEBUG_PZEM003
//       Serial.print("V (V):\t");
//       Serial.print(myPzem003Sensor.dcVoltage);
//       Serial.print("\t I (A):\t");
//       Serial.print(myPzem003Sensor.dcCurrent);
//       Serial.print("\t P (W):\t");
//       Serial.print(myPzem003Sensor.dcPower);
//       Serial.print("\t E (Wh):\t");
//       Serial.println(myPzem003Sensor.dcEnergy);
// #endif
//       break;
  }
  return true;
}

unsigned int pzem003Sensor::calc_CRC16(unsigned char *buf, int len) {
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

void pzem003Sensor::pwr_on() {
  // Set up the RS485 power output
  pinMode(RS485_PWR_PIN, OUTPUT);
  digitalWrite(RS485_PWR_PIN, HIGH);
  delay(POWER_SETTLE_TIME);
}

void pzem003Sensor::pwr_off() {
  digitalWrite(RS485_PWR_PIN, LOW);
  delay(POWER_SETTLE_TIME);
}

#endif