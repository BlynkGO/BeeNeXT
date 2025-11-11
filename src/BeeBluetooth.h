#ifndef __BEEUART_H__
#define __BEEUART_H__

#include <Arduino.h>
#include <BluetoothSerial.h>
#include "beenext_config.h"

#if BEENEXT_USE_BEEBLUETOOTH

#if defined(ESP32)

#define BEE_SLAVE     false
#define BEE_MASTER    true
extern BluetoothSerial _SerialBT;

namespace beebluetooth {
  // สำหรับ MCU
  void init(void(*fn)(String key, String value));

  // สำหรับ จอ BeeNeXT
  void init(String MCU_Address, void(*fn)(String key, String value));
  void hostAddress(String address);
  String address();

  bool connected();

  void print(String key, String value);
  void print(String key, float value, int decimalPlaces);
  void print(String key, double value, int decimalPlaces);
  void print(String key, int value);

  extern String _mcu_address;
  void _init(bool is_master, void(*fn)(String key, String value));
}

#endif // ESP32
#endif //BEENEXT_USE_BEEBLUETOOTH

#endif //__BEEUART_H__
