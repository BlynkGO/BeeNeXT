#include <BeeNeXT.h>
#include "BeeUART.h"

#if BEENEXT_USE_BEEUART

void(*_func_uart_recv_cb)(String key, String value);

#if ARDUINO_USB_CDC_ON_BOOT
#if ARDUINO_USB_MODE   // Hardware CDC mode
HWCDC * _hw_serial = NULL;
#else  // !ARDUINO_USB_MODE -- Native USB Mode
USBCDC * _hw_serial = NULL;
#endif
#else   // !ARDUINO_USB_CDC_ON_BOOT -- Serial is used from UART0
HardwareSerial * _hw_serial=NULL;
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  SoftwareSerial * _sw_serial=NULL;
#endif

void beeuart::init(void(*fn)(String key, String value)){
  _func_uart_recv_cb=fn;
  _hw_serial = &Serial;

  #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  if(_sw_serial !=NULL) { free(_sw_serial); }
  _sw_serial = NULL;
  #endif

  Serial.println("[BeeUART] on 'Serial'");
  static SoftTimer timer;
  timer.setInterval(1,[](){
    if(_hw_serial != NULL) {
      if(_hw_serial->available()){
        String raw = _hw_serial->readStringUntil('\n'); raw.trim();
        if(raw.startsWith("[BN]")) {
          raw.replace("[BN]", "");
          if(raw.indexOf(":")> 0) {
            String key    = raw.substring(0, raw.indexOf(":"));
            String value  = raw.substring(raw.indexOf(":") + 1);
            if(_func_uart_recv_cb) _func_uart_recv_cb(key, value);
          }
        }
      }
    }
  });
}

#if ARDUINO_USB_CDC_ON_BOOT
#if ARDUINO_USB_MODE   // Hardware CDC mode
// HWCDC * _hw_serial = NULL;
void beeuart::init(HWCDC *hw_serial, void(*fn)(String key, String value)){
  _func_uart_recv_cb=fn;
  _hw_serial = hw_serial;

  #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  if(_sw_serial !=NULL) { free(_sw_serial); }
  _sw_serial = NULL;
  #endif

  Serial.println("[BeeUART] on 'HWCDC Serial'");
  static SoftTimer timer;
  timer.setInterval(1,[](){
    if(_hw_serial != NULL) {
      if(_hw_serial->available()){
        String raw = _hw_serial->readStringUntil('\n'); raw.trim();
        if(raw.indexOf(":")> 0) {
          String key    = raw.substring(0, raw.indexOf(":"));
          String value  = raw.substring(raw.indexOf(":") + 1);
          if(_func_uart_recv_cb) _func_uart_recv_cb(key, value);
        }
      }
    }
  });
}
#else  // !ARDUINO_USB_MODE -- Native USB Mode
// USBCDC * _hw_serial = NULL;
void beeuart::init(USBCDC *hw_serial, void(*fn)(String key, String value)){
  _func_uart_recv_cb=fn;
  _hw_serial = hw_serial;

  #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  if(_sw_serial !=NULL) { free(_sw_serial); }
  _sw_serial = NULL;
  #endif

  Serial.println("[BeeUART] on 'USBCDC Serial'");
  static SoftTimer timer;
  timer.setInterval(1,[](){
    if(_hw_serial != NULL) {
      if(_hw_serial->available()){
        String raw = _hw_serial->readStringUntil('\n'); raw.trim();
        if(raw.indexOf(":")> 0) {
          String key    = raw.substring(0, raw.indexOf(":"));
          String value  = raw.substring(raw.indexOf(":") + 1);
          if(_func_uart_recv_cb) _func_uart_recv_cb(key, value);
        }
      }
    }
  });
}
#endif
#else   // !ARDUINO_USB_CDC_ON_BOOT -- Serial is used from UART0
// HardwareSerial * _hw_serial=NULL;
void beeuart::init(HardwareSerial *hw_serial, void(*fn)(String key, String value)){
  _func_uart_recv_cb=fn;
  _hw_serial = hw_serial;

  #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  if(_sw_serial !=NULL) { free(_sw_serial); }
  _sw_serial = NULL;
  #endif

  Serial.println("[BeeUART] on 'HardwareSerial'");
  static SoftTimer timer;
  timer.setInterval(1,[](){
    if(_hw_serial != NULL) {
      if(_hw_serial->available()){
        String raw = _hw_serial->readStringUntil('\n'); raw.trim();
        if(raw.indexOf(":")> 0) {
          String key    = raw.substring(0, raw.indexOf(":"));
          String value  = raw.substring(raw.indexOf(":") + 1);
          if(_func_uart_recv_cb) _func_uart_recv_cb(key, value);
        }
      }
    }
  });
}
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0) 
void beeuart::init(int8_t rx, int8_t tx, uint32_t baud, void(*fn)(String key, String value)) {
  _func_uart_recv_cb=fn;
  _hw_serial = NULL;

  if(_sw_serial != NULL) free(_sw_serial);
  _sw_serial = new SoftwareSerial(rx,tx);
  
  if(_sw_serial == NULL ) {
    Serial.println("[BeeUART] can't alloc SoftwareSerial!!");
    return;
  }

  Serial.printf("[BeeUART] on 'SoftwareSerial (rx %d, tx %d, baud %d)'\n", rx, tx, baud);
  _sw_serial->begin(baud);
  static SoftTimer timer;
  timer.setInterval(1,[](){
    if(_sw_serial != NULL) {
      if(_sw_serial->available()){
        String raw = _sw_serial->readStringUntil('\n'); raw.trim();
        if(raw.indexOf(":")> 0) {
          String key    = raw.substring(0, raw.indexOf(":"));
          String value  = raw.substring(raw.indexOf(":") + 1);
          if(_func_uart_recv_cb) _func_uart_recv_cb(key, value);
        }
      }
    }
  });
}
#endif


void beeuart::print(String key, String value){
  #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  if(_sw_serial != NULL) {
    _sw_serial->println(String("[BN]")+ key + ":"+ value);
  }else{
  #endif

  if(_hw_serial !=NULL) {
    _hw_serial->println(String("[BN]")+ key + ":"+ value);
  }

  #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  }
  #endif
}

void beeuart::print(String key, float value, int decimalPlaces){
  beeuart::print(key, String(value));
}

void beeuart::print(String key, double value, int decimalPlaces){
  beeuart::print(key, String(value, decimalPlaces));
}

void beeuart::print(String key, int value){
  beeuart::print(key, String(value));
}

#endif //#if BEENEXT_USE_BEEUART