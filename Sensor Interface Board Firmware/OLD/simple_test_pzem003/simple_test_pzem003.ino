#include <CustomSoftwareSerial.h>   // Use this for custom software serial
CustomSoftwareSerial* customSerial; // Declare serial
 
#define SOFT_SERIAL_RX    2   
#define SOFT_SERIAL_TX    3
#define RS485_PWR_PIN 4       // This is a power output for the RS485 port
//#define RS485_TX_PIN 3        // This is a Tx (out) for the RS485 port
//#define RS485_RX_PIN 2        // This is a Rx (in) for the RS485 port


uint8_t data_rs485[20];           // a byte array to hold incoming data
 
const uint8_t request_dc_v_1[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xCA};  // Read voltage from address 0x01
const uint8_t request_dc_v_2[] = {0x02, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xF9};  // Read voltage from address 0x02
 
void setup() 
{
  // initialize serial communication:
  Serial.begin(9600);
 
  // Set up the RS485 power output
  pinMode(RS485_PWR_PIN, OUTPUT);
  digitalWrite(RS485_PWR_PIN, HIGH);

  // Set the data rate for the SoftwareSerial port
  // For peacefair equipment: 8 bit, No parity, 2 Stop bits 
 
  customSerial = new CustomSoftwareSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX); // rx, tx
  customSerial->begin(9600, CSERIAL_8N1);         // Baud rate: 9600, configuration: CSERIAL_8N2
}
void loop()
{
   customSerial->write(request_dc_v_1, sizeof(request_dc_v_1));
   // Clear the data array:
   for (int x = 0; x < 20; x++)
   {
       data_rs485[x] = 0;
   }
   // MUST BE ABOUT 500mS short delay -  wait on serial to return the data.
   for (int x = 0; x < 500; x++)
   {
       delay(10);
   }
   uint8_t n = 0;
 
   // Wait for data back - if no data then this is empty
   while (customSerial->available())
    {
       // get the new byte:
       data_rs485[n] = (uint8_t)customSerial->read();
       n++;
    }
 
    // Display data:
    Serial.print(F("DATA:"));
    for (int x = 0; x < n; x++)
    {
      Serial.print(':');
      Serial.print(data_rs485[x], HEX);
    }
    Serial.println();
 
    delay(3000);   // Wait and then repeat
}
