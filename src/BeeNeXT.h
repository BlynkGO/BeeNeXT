#ifndef __BEENEXT_H__
#define __BEENEXT_H__

/** Major version number (X.x.x) */
#define BEENEXT_VERSION_MAJOR   2
/** Minor version number (x.X.x) */
#define BEENEXT_VERSION_MINOR   1
/** Patch version number (x.x.X) */
#define BEENEXT_VERSION_PATCH   0

#define BEENEXT_VERSION_TEXT    (String(BEENEXT_VERSION_MAJOR)+"."+String(BEENEXT_VERSION_MINOR)+"."+String(BEENEXT_VERSION_PATCH))

/**
 * Macro to convert IDF version number into an integer
 *
 * To be used in comparisons, such as BEENEXT_VERSION >= BEENEXT_VERSION_VAL(3, 0, 0)
 */
#define BEENEXT_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

/**
 * Current IDF version, as an integer
 *
 * To be used in comparisons, such as BLYNKGO_VERSION >= BLYNKGO_VERSION_VAL(3, 0, 0)
 */
#define BEENEXT_VERSION  BEENEXT_VERSION_VAL( BLYNKGO_VERSION_MAJOR, \
                                              BLYNKGO_VERSION_MINOR, \
                                              BLYNKGO_VERSION_PATCH)



// #include "../../config/blynkgo_config.h"
#define BEENEXT

/**********************************************
 * BeeNeXT config
 *********************************************/
#define BEENEXT_USE_SOFTTIMER           1
#define BEENEXT_USE_SOFTWARESERIAL      1

/**********************************************/
#if defined(BEENEXT) || BLYNKGO_USE_BEENEXT

#include <Arduino.h>

#if BEENEXT_USE_SOFTTIMER
  #include "libs/BeeNeXT_SoftTimer/BeeNeXT_SoftTimer.h"
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  #include "libs/SoftwareSerial/SoftwareSerial.h"
#endif

#define BEENEXT_DATA()          void BeeNeXT_onData()
#define SERIAL_DATA()           void BeeNeXT_onSerialData()
#define BEENEXT_CONNECTED()     void BeeNeXT_onConnected()
#define BEENEXT_DISCONNECTED()  void BeeNeXT_onDisconnected()
#ifdef __cplusplus
extern "C" {
#endif
  void BeeNeXT_NoOpCbk();
  BEENEXT_DATA();
  SERIAL_DATA();
  BEENEXT_CONNECTED();
  BEENEXT_DISCONNECTED();
#ifdef __cplusplus
}
#endif

#define BEENEXT_CONNECTION_TIMEOUT      3000

enum { EVENT_BEENEXT_CONNECTED, EVENT_BEENEXT_DISCONNECTED, EVENT_BEENEXT_DATA, EVENT_SERIAL_DATA };
typedef uint8_t beenect_event_t;

/**********************************************/
enum BeeNeXT_ReceiveState {
    BEENEXT_WAIT_FOR_PREHEADER,
    BEENEXT_WAIT_FOR_PREHEADER_CHECK,
    BEENEXT_WAIT_FOR_KEY_LENGTH,
    BEENEXT_WAIT_FOR_DATA_LENGTH,
    BEENEXT_WAIT_FOR_KEY,
    BEENEXT_WAIT_FOR_DATA,
    BEENEXT_WAIT_FOR_CHECKSUM
};

class BeeNeXT_class { //: public Print {
  public:
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
    BeeNeXT_class()  { }
#endif
    ~BeeNeXT_class();

    // API begin(..) นี้ จะใช้ HW Serial
    // ให้ HW Serial ที่ใช้ให้ HW Serial นั้นเริ่มทำงานก่อนเรียก API นี้
    void begin(HardwareSerial *serial=NULL );       // ต้อง begin() ของ serial มาก่อนเอาเอง ก่อนเรียกคำสั่งนี้
    void begin(HardwareSerial &serial );            // ต้อง begin() ของ serial มาก่อนเอาเอง ก่อนเรียกคำสั่งนี้
    inline HardwareSerial *HardSerial()             { return _hw_serial;    }

#if defined(__STM32F1__)
    // STM32
    void begin(USBSerial* serial);
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
    // API begin(..) นี้ จะใช้ SW Serial
    void begin(unsigned long baud, uint8_t rx, uint8_t tx);
    void begin(uint8_t rx, uint8_t tx);
    void begin(SoftwareSerial *softserial);         // ต้อง begin() ของ serial มาก่อนเอาเอง ก่อนเรียกคำสั่งนี้
    void begin(SoftwareSerial &softserial);         // ต้อง begin() ของ serial มาก่อนเอาเอง ก่อนเรียกคำสั่งนี้
    inline SoftwareSerial *SoftSerial()             { return _sw_serial; }
#endif
// #if CONFIG_IDF_TARGET_ESP32S3
//     void begin(unsigned long baud, uint8_t rx, uint8_t tx);
//     void begin(uint8_t rx, uint8_t tx);
// #endif

    void end();

    void update();
    void enable(bool en);

    inline bool isConnected()     { return _bee_connected;  }
    inline bool connected()       { return _bee_connected;  }

    // Method สำหรับรับค่า key ที่เข้ามาใน callback function
    inline String key()             { return String(_receivedKeyBuffer);}

    // Method สำหรับรับค่า data ที่เข้ามาใน callback function เป็น String
    inline String toString()        { return String((char*)_receivedDataBuffer); }
  
    // Method สำหรับรับค่า data ที่เข้ามาใน callback function เป็น uint8_t*
    inline uint8_t* data()          { return _receivedDataBuffer;       }
    inline uint16_t data_len()      { return _receivedDataLength;       }

    // Method สำหรับแปลงค่า data เป็น int
    inline int  toInt()             { return this->toString().toInt();  }
    inline bool toBool()            { return (bool) this->toInt();      }
    inline const char* c_str()      { return this->toString().c_str();  }
    inline float toFloat()          { return this->toString().toFloat();}

    // // Method สำหรับแปลงค่า data เป็น float
    // inline float toFloat() {  // ยังไม่ทำงาน
    //   float result;
    //   memcpy(&result, data(), sizeof(float));
    //   return result;
    // }


    // inline String data()          { return _data;                                                   }
    // inline void   data(String d)  { _data = d;  this->extract_key_value();                          }
    // inline String key()           { return _key;                                                    }
    // inline String value()         { return _value;                                                  }

    // inline String toString()      { return (_key=="")? _data                : _value;               }
    // inline int    toInt()         { return (_key=="")? _data.toInt()        : _value.toInt();       }
    // inline float  toFloat()       { return (_key=="")? _data.toFloat()      : _value.toFloat();     }
    // inline float  toDouble()      { return (_key=="")? _data.toDouble()     : _value.toDouble();    }
    // inline bool   toBool()        { return (_key=="")? (bool)_data.toInt()  : (bool)_value.toInt(); }
    // inline const char* c_str()    { return (_key=="")? _data.c_str()        : _value.c_str();       }
    // inline uint8_t* toBytes()     { return (_key=="")? (uint8_t*)_data.c_str()  : (uint8_t*)_value.c_str();       }


    // inline void send(String key, String value)                      { this->println(key+":"+value);                     }
    // void send(String key, String value);    // ไม่ได้ใช้ BeeNeXT.write(..) ใช้ HardwareSerial หรือ SoftwareSerial println(..)ออกไป
    // void send(String key, uint8_t* data, size_t data_len);
    // inline void send(String key, int value)                         { this->send(key,String(value));                    }
    // inline void send(String key, float  value, uint8_t decimal)     { this->send(key,String(value,(uint32_t)decimal));  }
    // inline void send(String key, double value, uint8_t decimal)     { this->send(key,String(value,(uint32_t)decimal));  }
    // inline void send(String key, const char* value)                 { this->send(key,String(value));                    }
    // inline void send(String key, char* value)                       { this->send(key,String(value));                    }
    // inline bool found_key()                                         { return (_key != "");                              }

    void send(String key, uint8_t *data, uint16_t data_len);
    inline void send(String key, String str)  { this->send(key, (uint8_t *)str.c_str(), str.length()); }
    inline void send(String key, int num)     { this->send(key, String(num)); }
    inline void send(String key, float f, unsigned int decimal=6)     { this->send(key, String(f, decimal)); }
    // inline void send(String key, float f) {  // ดูจะยังทำงานไม่ได้ ?
    //   // แปลงค่า float เป็น byte array
    //   byte floatData[sizeof(float)];
    //   memcpy(floatData, &f, sizeof(float));
    //   // ส่ง key และ byte array ของ float
    //   send(key, floatData, sizeof(float));

    //   Serial.println();
    //   for(int i=0;i< sizeof(float); i++){
    //     Serial.println(floatData[i], HEX);
    //   }
    // }
    // กำหนด callback function และเก็บไว้ในตัวแปร
    void setCallback(void (*callback)(void)) {
      this->callback = callback;
    }

    void event_send(beenect_event_t event);

    //virtual
    // size_t write(uint8_t);
    // size_t write(const uint8_t *buffer, size_t size);


    // don't call
    void   set_heartbeat(uint32_t heartbeat_interval);
    void   set_heartbeat_checker();
    bool   _bee_connected=false;
    uint32_t  _millis_heartbeat;
    static bool   _beenext_enable;

  private:
    // String _data;
    // String _key;
    // String _value;
    // void   extract_key_value();

    void   _extract_key_value(char c);

    const char preHeader[5] = "[BN]";

    char     _receivedPreHeader[5] = {0}; // สำหรับเก็บค่า pre-header ที่ส่งเข้ามา
    char     _receivedKeyBuffer[256]; // กำหนดขนาดสูงสุดของ key เป็น 256 (ความยาว key ที่รองรับได้มากที่สุด)
    uint8_t  _receivedDataBuffer[1024]; // กำหนดขนาดสูงสุดของ data เป็น 512 (ความยาวข้อมูลที่รองรับได้มากที่สุด)
    uint8_t  _receivedKeyLength;
    uint16_t _receivedDataLength;

    uint8_t  _receivedPreHeaderChars = 0;
    uint8_t  _receivedDataLengthBytes = 0;
    uint8_t  _receivedKeyChars = 0;
    uint8_t  _receivedDataChars = 0;
    uint8_t  _receivedChecksum = 0;
    BeeNeXT_ReceiveState _receiveState = BEENEXT_WAIT_FOR_PREHEADER;


    // ประกาศตัวแปร callback function ที่รับค่าเป็น pointer ไปยังฟังก์ชัน void
    void (*callback)(void) = nullptr;

    SoftTimer _timer_heartbeat;
    SoftTimer _timer_heartbeat_checker;
// #if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
    HardwareSerial * _hw_serial=NULL;
// #endif

#if defined(__STM32F1__)
    // STM32
    USBSerial*       _usb_serial=NULL;
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
    SoftwareSerial * _sw_serial=NULL;
    bool  _is_swserial_alloced=false;
#endif
};
/**********************************************/
extern BeeNeXT_class BeeNeXT;

#endif // #if defined(BEENEXT) || BLYNKGO_USE_BEENEXT
#endif //__BEENEXT_H__
