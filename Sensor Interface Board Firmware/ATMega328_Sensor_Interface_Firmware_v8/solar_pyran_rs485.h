#ifdef SOLAR_RS485_SENSOR

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

class pyr20Sensor
{
public:
	void begin();
  bool readIrradiance(byte ID);   // Where ID is the ID of the unit (default 1, can be 1-255)
  void changeID(byte IDold, byte IDnew);  // Conver the ID from IDold to IDnew
  uint32_t pyranInt;    // Holds the data
#ifdef I2C_OLED_DISPLAY
  // These routines added for an OLED display to show the data.
  


#endif
private:
  unsigned int calc_CRC16(unsigned char *buf, int len); 
};
extern pyr20Sensor pyr20SensorRS485;

#endif