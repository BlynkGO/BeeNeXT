#ifndef __BEEUART_H__
#define __BEEUART_H__

#include "beenext_config.h"

#if BEENEXT_USE_BEEUART

#include <HardwareSerial.h>
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0) 

#if defined(ESP8266)
#include <SoftwareSerial.h>   // ใช้ของ Arduino ESP8266 core 3.1.2 ที่มีให้แล้วแทน
#else
#include "lib/SoftwareSerial/SoftwareSerial.h"
#endif
#endif //BEENEXT_USE_SOFTWARESERIAL

namespace beeuart {
  void init(void(*fn)(String key, String value));

#if ARDUINO_USB_CDC_ON_BOOT
  #if ARDUINO_USB_MODE   // Hardware CDC mode
    // HWCDC * _hw_serial = NULL;
    void init(HWCDC *hw_serial, void(*fn)(String key, String value));
  #else  // !ARDUINO_USB_MODE -- Native USB Mode
    // USBCDC * _hw_serial = NULL;
    void init(USBCDC *hw_serial, void(*fn)(String key, String value));
  #endif
#else   // !ARDUINO_USB_CDC_ON_BOOT -- Serial is used from UART0
  // HardwareSerial * _hw_serial=NULL;
  void init(HardwareSerial *hw_serial, void(*fn)(String key, String value));
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0) 
  void init(int8_t rx, int8_t tx, uint32_t baud, void(*fn)(String key, String value));
  inline void init(int8_t rx, int8_t tx, void(*fn)(String key, String value)){
    beeuart::init(rx,tx,9600, fn);
  }
#endif
  void print(String key, String value);
  void print(String key, float value, int decimalPlaces);
  void print(String key, double value, int decimalPlaces);
  void print(String key, int value);
}


#endif //BEENEXT_USE_BEEUART

#endif //__BEEUART_H__