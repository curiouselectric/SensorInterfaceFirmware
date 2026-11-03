#define PZEM_003_RS485_SENSOR

#ifdef PZEM_003_RS485_SENSOR

/****************************************************************************/
/***        Including Files                                               ***/
/****************************************************************************/
#include <Arduino.h>
#include <SoftwareSerial.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Class Definition                                              ***/
/****************************************************************************/

class pzem003Sensor {
public:
  void begin();
  bool readVoltage(byte ID);  // Where ID is the ID of the unit (default 1, can be 1-255)
  bool readCurrent(byte ID);  // Where ID is the ID of the unit (default 1, can be 1-255)
  bool readPower(byte ID);    // Where ID is the ID of the unit (default 1, can be 1-255)
  bool readEnergy(byte ID);   // Where ID is the ID of the unit (default 1, can be 1-255)
  bool readAll(byte ID);      // Where ID is the ID of the unit (default 1, can be 1-255)
  float dcVoltage;
  float dcCurrent;
  float dcPower;
  float dcEnergy;
  void pwr_on();
  void pwr_off();

private:
  unsigned int calc_CRC16(unsigned char *buf, int len);
  bool getRequestData(char dataType);
  void createRequest(byte buf[], int len);
};
extern pzem003Sensor myPzem003Sensor;

#endif