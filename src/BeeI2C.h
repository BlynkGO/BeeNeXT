#ifndef __BEE_I2C_H__
#define __BEE_I2C_H__


#include "beenext_config.h"

#if BEENEXT_USE_BEEI2C
#include <Wire.h>


namespace McuI2C_Master {
  void init(void(*fn)(String key, String value));
#if defined(ESP8266) || defined(ESP32)
  void init(uint8_t sda, uint8_t scl, void(*fn)(String key, String value));
#endif
  void print(String key, String value);
  void print(String key, float value, int decimalPlaces);
  void print(String key, double value, int decimalPlaces);
  void print(String key, int value);
}

#if defined(ESP32)
namespace BeeI2C_Slave {
  void init(uint8_t sda, uint8_t scl, void(*fn)(String key, String value));

#if defined(BEENEXT_2_4) || defined(BEENEXT_2_4C) || defined(BEENEXT_3_2) || defined(BEENEXT_3_2C) || defined(BEENEXT_3_5)  || defined(BEENEXT_3_5C)
  inline void init(void(*fn)(String key, String value)){
    BeeI2C_Slave::init(21, 22, fn);
  }
#elif defined(BEENEXT_4_3) || defined(BEENEXT_4_3C) || defined(BEENEXT_4_3IPS) || defined(BEENEXT_5_0IPS)  || defined(BEENEXT_7_0IPS)
inline void init(void(*fn)(String key, String value)){
  BeeI2C_Slave::init(17, 18, fn);
}
#endif

  void print(String key, String value);
  void print(String key, float value, int decimalPlaces);
  void print(String key, double value, int decimalPlaces);
  void print(String key, int value);
}
#endif // #if defined(ESP32)


#endif // #if BEENEXT_USE_BEEI2C

#endif // #ifndef __BEE_I2C_H__