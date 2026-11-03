#include "config.h"

#if defined(PZEM_003_RS485_SENSOR) ||  defined(PZEM_017_RS485_SENSOR) ||  defined(PZEM_014_RS485_SENSOR ) ||  defined(PZEM_016_RS485_SENSOR)
#include <SoftwareSerial.h>

SoftwareSerial mod(RS485_RX_PIN, RS485_TX_PIN);  // Software serial for RS485 communication

pzemSensor myPzemSensor;

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

byte pzemRequestNoCRC[6];
byte pzemRequest[8];
byte pzemResponse[16];  // a byte array to hold incoming data

/****************************************************************************/
/***       Class member Functions                                         ***/
/****************************************************************************/

void pzemSensor::begin(void) {

  // Set up the RS485 power output
  pwr_on();
  // Set the data rate for the SoftwareSerial port
  // For peacefair equipment: 8 bit, No parity, 1 Stop bits
  /* Start Serial */
  mod.begin(sensorBaudRate);  // Initialize software serial communication at 9600 baud rate
  mod.setTimeout(500);        // Set timeout to 500mS
  pwr_off();
}

bool pzemSensor::readVoltage(byte ID) {
  // Add the module ID to the request
  pzemRequestNoCRC[0] = ID;
  for (int i = 0; i < sizeof(request_dc_v); i++) {
    pzemRequestNoCRC[i + 1] = request_dc_v[i];
  }
  // Request the voltage:
  createRequest(pzemRequestNoCRC, sizeof(pzemRequestNoCRC));
  mod.write(pzemRequest, sizeof(pzemRequest));
  delay(200);  // Wait a bit for a response...
  if (getRequestData('v') == true) {
    return (true);
  } else {
    return (false);
  }
}

bool pzemSensor::readCurrent(byte ID) {
  // Add the module ID to the request
  pzemRequestNoCRC[0] = ID;
  for (int i = 0; i < sizeof(request_dc_i); i++) {
    pzemRequestNoCRC[i + 1] = request_dc_i[i];
  }
  // Request the voltage:
  createRequest(pzemRequestNoCRC, sizeof(pzemRequestNoCRC));
  mod.write(pzemRequest, sizeof(pzemRequest));
  delay(100);  // Wait a bit for a response...
  if (getRequestData('i') == true) {
    return (true);
  } else {
    return (false);
  }
}

bool pzemSensor::readPower(byte ID) {
  // Add the module ID to the request
  pzemRequestNoCRC[0] = ID;
  for (int i = 0; i < sizeof(request_dc_power); i++) {
    pzemRequestNoCRC[i + 1] = request_dc_power[i];
  }
  // Request the voltage:
  createRequest(pzemRequestNoCRC, sizeof(pzemRequestNoCRC));
  mod.write(pzemRequest, sizeof(pzemRequest));
  delay(100);  // Wait a bit for a response...
  if (getRequestData('p') == true) {
    return (true);
  } else {
    return (false);
  }
}

bool pzemSensor::readEnergy(byte ID) {
  // Add the module ID to the request
  pzemRequestNoCRC[0] = ID;
  for (int i = 0; i < sizeof(request_dc_energy); i++) {
    pzemRequestNoCRC[i + 1] = request_dc_energy[i];
  }
  // Request the voltage:
  createRequest(pzemRequestNoCRC, sizeof(pzemRequestNoCRC));
  mod.write(pzemRequest, sizeof(pzemRequest));
  delay(100);  // Wait a bit for a response...
  if (getRequestData('e') == true) {
    return (true);
  } else {
    return (false);
  }
}

#if defined(PZEM_014_RS485_SENSOR ) ||  defined(PZEM_016_RS485_SENSOR)



#endif
// bool pzemSensor::readAll(byte ID) {
//   // Add the module ID to the request
//   pzemRequestNoCRC[0] = ID;
//   for (int i = 0; i < sizeof(request_dc_all); i++) {
//     pzemRequestNoCRC[i + 1] = request_dc_all[i];
//   }
//   // Request the voltage:
//   createRequest(pzemRequestNoCRC, sizeof(pzemRequestNoCRC));
//   mod.write(pzemRequest, sizeof(pzemRequest));
//   delay(100);  // Wait a bit for a response...
//   if (getRequestData('a') == true) {
//     return (true);
//   } else {
//     return (false);
//   }
// }

void pzemSensor::createRequest(byte buf[], int len) {
  // Calculate the CRC16 value (depends upon the sensor ID)
  unsigned int CRCvalue = calc_CRC16(buf, len);
  for (int i = 0; i < len; i++) {
    pzemRequest[i] = buf[i];
  }
  pzemRequest[len] = highByte(CRCvalue);
  pzemRequest[len + 1] = lowByte(CRCvalue);

#ifdef DEBUG_PZEM
  // Debug output
  Serial.print("REQ: ");
  for (auto x : pzemRequest)  // for each element 'x' in the array 'pzemRequest'
  {
    Serial.print(x, HEX);
    Serial.print(":");
  }
  Serial.println();
//End of debug
#endif
}

bool pzemSensor::getRequestData(char dataType) {
  // Read the response from the sensor
  byte index = 0;
  while (mod.available()) {
    pzemResponse[index] = mod.read();
    if (pzemResponse[index] == '\n') {
      // got end of line character so break out
      break;
    }
#ifdef DEBUG_PZEM
    // Debug:
    // Show the data recieved:
    Serial.print(pzemResponse[index], HEX);  // Print the received byte in HEX format
    Serial.print(" ");
// End of debug
#endif
    index++;
  }
#ifdef DEBUG_PZEM
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
      dcVoltage = (int(pzemResponse[3] << 8 | pzemResponse[4]) / 100.0);
      break;
    case (char)'i':
      //Parse and calculate the current data:
      dcCurrent = (int(pzemResponse[3] << 8 | pzemResponse[4]) / 100.0);
      break;
    case (char)'p':
      //Parse and calculate the power data:
      long int dcPower_Int = 0;
      dcPower_Int = dcPower_Int + long(pzemResponse[3]);
      dcPower_Int << 8;
      dcPower_Int = dcPower_Int + long(pzemResponse[4]);
      dcPower_Int << 8;
      dcPower_Int = dcPower_Int + long(pzemResponse[5]);
      dcPower_Int << 8;
      dcPower_Int = dcPower_Int + long(pzemResponse[6]);
      dcPower = ((float)dcPower_Int / 10.0);
      break;
    case (char)'e':
      //Parse and calculate the energy data:
      long int dcEnergy_Int = 0;
      dcEnergy_Int = dcEnergy_Int + long(pzemResponse[3]);
      dcEnergy_Int << 8;
      dcEnergy_Int = dcEnergy_Int + long(pzemResponse[4]);
      dcEnergy_Int << 8;
      dcEnergy_Int = dcEnergy_Int + long(pzemResponse[5]);
      dcEnergy_Int << 8;
      dcEnergy_Int = dcEnergy_Int + long(pzemResponse[6]);
      dcEnergy = ((float)dcEnergy_Int);
      break;

#if defined(PZEM_014_RS485_SENSOR ) ||  defined(PZEM_016_RS485_SENSOR)      
    // case (char)'f':
    //   //Parse and calculate the frequency data:
    //   acFreq = (int(pzemResponse[7] << 8 | pzemResponse[8]) / 100.0);
    //   break;
    // case (char)'q':
    //   acPowerFactor = (int(pzemResponse[9] << 8 | pzemResponse[10]) / 100.0);
    //   break;
#endif

//     case (char)'a':
//       dcVoltage = (int(pzemResponse[3] << 8 | pzemResponse[4]) / 100.0);
//       dcCurrent = (int(pzemResponse[5] << 8 | pzemResponse[6]) / 100.0);
//       long int dc_Int = 0;
//       dc_Int = dc_Int + long(pzemResponse[7]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzemResponse[8]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzemResponse[9]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzemResponse[10]);
//       dcPower = ((float)dc_Int / 10.0);
//       dc_Int = 0;
//       dc_Int = dc_Int + long(pzemResponse[11]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzemResponse[12]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzemResponse[13]);
//       dc_Int << 8;
//       dc_Int = dc_Int + long(pzemResponse[14]);
//       dcEnergy = ((float)dc_Int);
// #ifdef DEBUG_PZEM
//       Serial.print("V (V):\t");
//       Serial.print(myPzemSensor.dcVoltage);
//       Serial.print("\t I (A):\t");
//       Serial.print(myPzemSensor.dcCurrent);
//       Serial.print("\t P (W):\t");
//       Serial.print(myPzemSensor.dcPower);
//       Serial.print("\t E (Wh):\t");
//       Serial.println(myPzemSensor.dcEnergy);
// #endif
//       break;
  }
  return true;
}

unsigned int pzemSensor::calc_CRC16(unsigned char *buf, int len) {
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

void pzemSensor::pwr_on() {
  // Set up the RS485 power output
  pinMode(RS485_PWR_PIN, OUTPUT);
  digitalWrite(RS485_PWR_PIN, HIGH);
  delay(POWER_SETTLE_TIME);
}

void pzemSensor::pwr_off() {
  digitalWrite(RS485_PWR_PIN, LOW);
  delay(POWER_SETTLE_TIME);
}

#endif