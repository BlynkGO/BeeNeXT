#ifndef __BEENEXT_H__
#define __BEENEXT_H__

#define BEENEXT_VERSION     1.0.3

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
#include "BeeNeXT_command.h"

#if BEENEXT_USE_SOFTTIMER
  #include "libs/BeeNeXT_SoftTimer/BeeNeXT_SoftTimer.h"
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  #include "libs/SoftwareSerial/SoftwareSerial.h"
#endif

#define BEENEXT_DATA()      void BeeNeXT_onData()
#define SERIAL_DATA()       void BeeNeXT_onSerialData()
#ifdef __cplusplus
extern "C" {
#endif
  void BeeNeXT_NoOpCbk();
  BEENEXT_DATA();
  SERIAL_DATA();
#ifdef __cplusplus
}
#endif
/**********************************************/

class BeeNeXT_class : public Print {
  public:
    BeeNeXT_class()   {}
    ~BeeNeXT_class();

    // API begin(..) นี้ จะใช้ HW Serial
    // ให้ HW Serial ที่ใช้ให้ HW Serial นั้นเริ่มทำงานก่อนเรียก API นี้
    void begin(HardwareSerial *serial=NULL );  
    void begin(HardwareSerial &serial );
    inline HardwareSerial *HardSerial()             { return _hw_serial; }
    
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
    // API begin(..) นี้ จะใช้ SW Serial
    void begin(unsigned long baud, uint8_t rx, uint8_t tx);
    void begin(uint8_t rx, uint8_t tx);
    inline SoftwareSerial *SoftSerial()             { return _sw_serial; }
#endif
// #if CONFIG_IDF_TARGET_ESP32S3
//     void begin(unsigned long baud, uint8_t rx, uint8_t tx);
//     void begin(uint8_t rx, uint8_t tx);
// #endif

    void end();

    void update();

    inline String data()          { return _data;  }
    inline String key()           { return _key;   }
    inline String value()         { return _value; }

    inline String toString()      { return (_key=="")? _data                : _value;               }
    inline int    toInt()         { return (_key=="")? _data.toInt()        : _value.toInt();       }
    inline float  toFloat()       { return (_key=="")? _data.toFloat()      : _value.toFloat();     }
    inline float  toDouble()      { return (_key=="")? _data.toDouble()     : _value.toDouble();    }
    inline bool   toBool()        { return (_key=="")? (bool)_data.toInt()  : (bool)_value.toInt(); }
    inline const char* c_str()    { return (_key=="")? _data.c_str()        : _value.c_str();       }

    void command(uint16_t cmd);

    inline void send(String key, String value)    { this->println(key+":"+value); }

    //virtual
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
  private:
    String _data;
    String _key;
    String _value;
    void   extract_key_value();
    HardwareSerial * _hw_serial=NULL;
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
    SoftwareSerial * _sw_serial=NULL;
    bool  _is_swserial_alloced=false;
#endif
};
/**********************************************/
extern BeeNeXT_class BeeNeXT;

#endif // #if defined(BEENEXT) || BLYNKGO_USE_BEENEXT
#endif //__BEENEXT_H__
