#include "BeeNeXT.h"

BeeNeXT_Class BeeNeXT;

/******************************************************/
void BeeNeXT_NoOpCbk() {}
BEENEXT_DATA()          __attribute__((weak, alias("BeeNeXT_NoOpCbk")));
BEENEXT_CONNECTED()     __attribute__((weak, alias("BeeNeXT_NoOpCbk")));
BEENEXT_DISCONNECTED()  __attribute__((weak, alias("BeeNeXT_NoOpCbk")));
/******************************************************/
#if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER
static SoftTimer timer_delay;
#endif
bool BeeNeXT_Class::_beenext_enable = true;

void BeeNeXT_Class::begin(HardwareSerial *hw_serial){
  this->end();
  Serial.println("[BeeNeXT] on HardwareSerial");
  _hw_serial = hw_serial;  // ไม่มีการ serial begin() มาก่อนเอาเอง
  _hw_serial->setTimeout(50);
  _hw_serial->flush();

#if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER
  this->init_heartbeat();
#endif // #if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER  

}

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
void BeeNeXT_Class::begin(SoftwareSerial *sw_serial){
  this->end();
  Serial.println("[BeeNeXT] on SoftwareSerial");
  _sw_serial = sw_serial;  // Software Serial ต้อง begin มาเอง
  _sw_serial->setTimeout(50);
#if defined(ESP8266) || defined(ESP32)
#else
  _sw_serial->flush();
#endif

#if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER
  this->init_heartbeat();
#endif // #if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER  
}

void BeeNeXT_Class::begin(unsigned long baud, uint8_t rx, uint8_t tx){
  this->end();
  _sw_serial =  new SoftwareSerial();
  _is_swserial_alloced = true;
  _sw_serial->begin(baud, rx, tx );
  _sw_serial->setTimeout(50);
#if defined(ESP8266) || defined(ESP32)
#else
  _sw_serial->flush();
#endif

#if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER
  this->init_heartbeat();
#endif // #if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER  
}
#endif

void BeeNeXT_Class::end(){
  if( _hw_serial != NULL) { 
    _hw_serial->end(); 
    _hw_serial = NULL;
  }
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  if( _sw_serial != NULL ){
    _sw_serial->end();
    if(_is_swserial_alloced){
      free(_sw_serial); _sw_serial = NULL;
      _is_swserial_alloced = false;
    }
  }
#endif
}

#if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER
void BeeNeXT_Class::init_heartbeat(){
  this->set_heartbeat(1000);
  this->set_heartbeat_checker();
  #if defined(ESP8266) || defined(ESP32)
    timer_delay.delay(1000,[](){ if(!BeeNeXT.connected()) BeeNeXT_onDisconnected(); });  
  #else
    delay(1000);
    if(!BeeNeXT.connected()) BeeNeXT_onDisconnected();
  #endif
}

void BeeNeXT_Class::stop_heartbeat(){
  _timer_heartbeat.del();
}

void BeeNeXT_Class::set_heartbeat(uint32_t heartbeat_interval){
  _timer_heartbeat.setInterval(heartbeat_interval,[](){
    if(BeeNeXT_Class::_beenext_enable) {
      if(BeeNeXT.heartbeat())
        BeeNeXT.send("_bhb_", true);
    }
  });
}

void BeeNeXT_Class::set_heartbeat_checker(){
  _timer_heartbeat_checker.setInterval(BEENEXT_CONNECTION_TIMEOUT,[](){
    if(millis()> BeeNeXT._millis_heartbeat + BEENEXT_CONNECTION_TIMEOUT){
      if(BeeNeXT._bee_connected) {
        BeeNeXT._bee_connected = false;
        BeeNeXT_onDisconnected();
        if( BeeNeXT.heartbeat() )
          BeeNeXT.set_heartbeat(1000);
      }
    }
  }, true);
}
#endif // #if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER

void BeeNeXT_Class::send(String key, uint8_t *data, uint16_t data_len){
  if(_hw_serial != NULL) {
    // ส่ง pre-header
    _hw_serial->write(_preHeader, sizeof(_preHeader) - 1);

    // ส่งความยาวของ key (1 byte)
    uint8_t  keyLength = key.length();
    if(keyLength > 32) { keyLength = 32; key[keyLength] = '\0'; }
    _hw_serial->write(keyLength);

    // ส่งความยาวของ data (2 byte)
    _hw_serial->write((uint8_t *)&data_len, 2);

    // ส่ง key
    _hw_serial->write(key.c_str(), keyLength);

    // ส่ง data
    _hw_serial->write(data, data_len);

    // คำนวณ checksum crc16 และ ส่ง checksum
    uint16_t  _checksum = CRC16( 0, (uint8_t*)_preHeader, sizeof(_preHeader) - 1);
              _checksum = CRC16( _checksum, keyLength);
              _checksum = CRC16( _checksum, (uint8_t *)&data_len  , sizeof(data_len));
              _checksum = CRC16( _checksum, (uint8_t *)key.c_str(), keyLength);
              _checksum = CRC16( _checksum, (uint8_t *)data       , data_len);
    
    _hw_serial->write((uint8_t *)&_checksum, 2);
    _hw_serial->println();

    // Serial.printf("[Sender] checksum : %d\n",_checksum);
    delay(1);
  }
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  else if(_sw_serial != NULL) {
    // ส่ง pre-header
    _sw_serial->write(_preHeader, sizeof(_preHeader) - 1);

    // ส่งความยาวของ key (1 byte)
    uint8_t  keyLength = key.length();
    if(keyLength > 32) { keyLength = 32; key[keyLength] = '\0'; }
    _sw_serial->write(keyLength);

    // ส่งความยาวของ data (2 byte)
    _sw_serial->write((uint8_t *)&data_len, 2);

    // ส่ง key
    _sw_serial->write(key.c_str(), keyLength);

    // ส่ง data
    _sw_serial->write(data, data_len);

    // คำนวณ checksum crc16 และ ส่ง checksum
    uint16_t  _checksum = CRC16( 0, (uint8_t*)_preHeader, sizeof(_preHeader) - 1);
              _checksum = CRC16( _checksum, keyLength);
              _checksum = CRC16( _checksum, (uint8_t *)&data_len  , sizeof(data_len));
              _checksum = CRC16( _checksum, (uint8_t *)key.c_str(), keyLength);
              _checksum = CRC16( _checksum, (uint8_t *)data       , data_len);
    
    _sw_serial->write((uint8_t *)&_checksum, 2);
    _sw_serial->println();

    // Serial.printf("[Sender] checksum : %d\n",_checksum);

    delay(1);
  }
#endif
}


uint16_t BeeNeXT_Class::CRC16(uint16_t crc /*0 = init*/, uint8_t *data, size_t length) {
  if(crc==0) crc = 0xFFFF;
  for (size_t i = 0; i < length; i++) {
    crc ^= (uint16_t)data[i];  // Convert uint8_t to uint16_t before XOR
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x0001) { crc >>= 1;  crc ^= 0xA001;  } 
      else              { crc >>= 1;                  }
    }
  }
  return crc;
}

uint16_t BeeNeXT_Class::CRC16(uint16_t crc /*0 = init*/, uint8_t data) { 
  return CRC16(crc, &data, 1);
}

void BeeNeXT_Class::enable(bool en){
  BeeNeXT_Class::_beenext_enable = !!en;
  Serial.println((BeeNeXT_Class::_beenext_enable)? "[BeeNeXT] enable" : "[BeeNeXT] disable");
}

void BeeNeXT_Class::update(){
#if BEENEXT_USE_SOFTTIMER
  SoftTimer::run();
#endif

  if(!this->enable()) return;

  if(_hw_serial != NULL) {
    while (_hw_serial->available() > 0) {
      char ch = _hw_serial->read();
      this->_updateChar(ch);
    }
// #if defined(ESP8266) || defined(ESP32)
// #else
    _hw_serial->flush();
// #endif
  }
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  else if(_sw_serial != NULL) {
    while (_sw_serial->available() > 0) {
      char ch = _sw_serial->read();
      this->_updateChar(ch);
    }
#if defined(ESP8266) || defined(ESP32)
#else
    _sw_serial->flush();
#endif
  }
#endif

}

void BeeNeXT_Class::_updateChar(char ch){
  static uint8_t  _recv_PreHeaderChars = 0;
  static uint8_t  _recv_DataLengthBytes = 0;
  static uint8_t  _recv_KeyChars = 0;
  static uint8_t  _recv_DataChars = 0;
  static uint16_t _recv_Checksum = 0;
  static uint8_t  _recv_Checksum_Byte = 0;

  static BeeNeXT_ReceiveState _receiveState = BEENEXT_WAIT_FOR_PREHEADER;

  switch (_receiveState) {
    case BEENEXT_WAIT_FOR_PREHEADER:
      if (ch == '[') { // เมื่อตัวแรกของ pre-header เป็น '['
        _recv_PreHeader[0]   = ch; // เก็บตัวอักษรแรกของ pre-header ไว้
        _recv_PreHeaderChars = 1;  // เริ่มต้นนับตัวอักษรแรกของ pre-header
        _receiveState = BEENEXT_WAIT_FOR_PREHEADER_CHECK;
      } else {
        // ถ้าไม่ใช่ '[' ให้เริ่มต้นการรับ pre-header ใหม่
        _recv_PreHeaderChars = 0;
      }
      break;
    case BEENEXT_WAIT_FOR_PREHEADER_CHECK:
      _recv_PreHeader[_recv_PreHeaderChars] = ch; // เก็บตัวอักษรถัดไปของ pre-header ไว้
      _recv_PreHeaderChars++;
      if (_recv_PreHeaderChars == sizeof(_preHeader) - 1) {
        if (memcmp(_recv_PreHeader, _preHeader, sizeof(_preHeader) - 1) == 0) {
          // pre-header ถูกต้อง
          // Serial.println("[BeeNeXT] pre-header match!");
          _receiveState = BEENEXT_WAIT_FOR_KEY_LENGTH;
        } else {
          // pre-header ไม่ถูกต้อง
          // ทำการตรวจสอบและหากเกิดข้อผิดพลาดที่นี่
          // ...
          // เมื่อประมวลผลเสร็จสิ้น กลับไปรอรับข้อมูลใหม่
          _receiveState = BEENEXT_WAIT_FOR_PREHEADER;
        }
        _recv_PreHeaderChars = 0; // รีเซ็ตการนับตัวอักษร pre-header ใหม่
      }
      break;
    case BEENEXT_WAIT_FOR_KEY_LENGTH:
      _recv_KeyLength = ch;
      _recv_DataLengthBytes = 0;
      // Serial.printf("[BeeNeXT] key len : %d\n", _recv_KeyLength);
      _receiveState = BEENEXT_WAIT_FOR_DATA_LENGTH;
      break;
    case BEENEXT_WAIT_FOR_DATA_LENGTH:
      if (_recv_DataLengthBytes == 0) {
        ((uint8_t *)&_recv_DataLength)[0] = ch;
        _recv_DataLengthBytes = 1;
      } else {
        ((uint8_t *)&_recv_DataLength)[1] = ch;
        _recv_KeyChars = 0;
        memset(_recv_KeyBuffer, sizeof(_recv_KeyBuffer), 0); // ล้าง _recv_KeyBuffer
        // Serial.printf("[BeeNeXT] data len : %d\n", _recv_DataLength);
        _receiveState = BEENEXT_WAIT_FOR_KEY;
      }
      break;
    case BEENEXT_WAIT_FOR_KEY:
      _recv_KeyBuffer[_recv_KeyChars] = ch;
      _recv_KeyChars++;
      if (_recv_KeyChars == _recv_KeyLength) {
        _recv_KeyBuffer[_recv_KeyLength] = 0;  // ตัวปิด string c array
        _recv_DataChars = 0;
        // Serial.printf("[BeeNeXT] key : %s\n", _recv_KeyBuffer);
        memset(_recv_DataBuffer, sizeof(_recv_DataBuffer), 0); // ล้าง _recv_DataBuffer
        _receiveState = BEENEXT_WAIT_FOR_DATA;
      }
      break;
    case BEENEXT_WAIT_FOR_DATA:
      _recv_DataBuffer[_recv_DataChars] = ch;
      _recv_DataChars++;
      if (_recv_DataChars == _recv_DataLength) {
        // if( _recv_DataLength == 4) {
        //   int32_t num = *(int32_t*)_recv_DataBuffer;
        //   Serial.printf("[BeeNeXT] data : %d\n", num);
        // }
        _recv_Checksum_Byte = 0;
        _receiveState = BEENEXT_WAIT_FOR_CHECKSUM;
      }
      break;
    case BEENEXT_WAIT_FOR_CHECKSUM:
      if (_recv_Checksum_Byte == 0) {
        _recv_Checksum = ch;  // เก็บ ch ไว้ที่ byte แรก
        _recv_Checksum_Byte = 1;  // กำหนดให้เก็บใน byte ถัดไป
      } else if (_recv_Checksum_Byte == 1) {
        _recv_Checksum |= (uint16_t)ch << 8;  // เก็บ ch ไว้ที่ byte ที่สอง
        _recv_Checksum_Byte = 0;  // กำหนดให้เก็บใน byte แรกในรอบถัดไป

        // คำนวณ checksum ของ pre-header
        uint16_t  _checksum = CRC16( 0        , (uint8_t*)_preHeader, sizeof(_preHeader) - 1);
                  _checksum = CRC16( _checksum, _recv_KeyLength);
                  _checksum = CRC16( _checksum, (uint8_t *)&_recv_DataLength, sizeof(_recv_DataLength));
                  _checksum = CRC16( _checksum, (uint8_t *) _recv_KeyBuffer , _recv_KeyLength);
                  _checksum = CRC16( _checksum, (uint8_t *) _recv_DataBuffer, _recv_DataLength);

        // Serial.print("[recv checksum] ");
        // Serial.println(_recv_Checksum);
        // Serial.print("[calc checksum] ");
        // Serial.println(_checksum);
        
        if (_recv_Checksum == _checksum) {
          // if (callback) {
          //   callback();
          // }
          // Serial.println("[recv] checksum OK");
          // Serial.printf("[Recv] key : %s; data len : %d\n", _recv_KeyBuffer, _recv_DataLength);

#if BEENEXT_USE_HEARTBEAT && BEENEXT_USE_SOFTTIMER
          if( this->key() == "_bhb_"){
            // Serial.println("[_bhb_] found");
            _millis_heartbeat = millis();
            if( this->toBool() != _bee_connected ) {
              _bee_connected = this->toBool();
              if(_bee_connected ) {
                BeeNeXT_onConnected();
                this->set_heartbeat(BEENEXT_CONNECTION_TIMEOUT);
              }else {
                BeeNeXT_onDisconnected();
                this->set_heartbeat(1000);
              }
            }
          }else
#endif
            BeeNeXT_onData();
        } else {
          // Checksum ไม่ถูกต้อง
        }
        // เมื่อประมวลผลเสร็จสิ้น กลับไปรอรับข้อมูลใหม่
        _receiveState = BEENEXT_WAIT_FOR_PREHEADER;
      }else{
        _receiveState = BEENEXT_WAIT_FOR_PREHEADER;
      }
      break;
    }
}

// int8_t  BeeNeXT_Class::toInt8(){
//   if( _recv_DataLength == 1) {
//     int8_t n = *(int8_t*) _recv_DataBuffer;
//     return (int8_t)n;
//   }else
//   if( _recv_DataLength == 2) {
//     int16_t n = *(int16_t*) _recv_DataBuffer;
//     return (int8_t)n;
//   }else
//   if( _recv_DataLength == 4) {
//     int32_t n = *(int32_t*) _recv_DataBuffer;
//     return (int8_t)n;
//   }else
//   if( _recv_DataLength == 8) {
//     int64_t n = *(int64_t*) _recv_DataBuffer;
//     return (int8_t)n;
//   }
// }

// int16_t BeeNeXT_Class::toInt16(){
//   if( _recv_DataLength == 1) {
//     int8_t n = *(int8_t*) _recv_DataBuffer;
//     return (int16_t)n;
//   }else
//   if( _recv_DataLength == 2) {
//     int16_t n = *(int16_t*) _recv_DataBuffer;
//     return (int16_t)n;
//   }else
//   if( _recv_DataLength == 4) {
//     int32_t n = *(int32_t*) _recv_DataBuffer;
//     return (int16_t)n;
//   }else
//   if( _recv_DataLength == 8) {
//     int64_t n = *(int64_t*) _recv_DataBuffer;
//     return (int16_t)n;
//   }
// }

// int32_t BeeNeXT_Class::toInt32(){
//   if( _recv_DataLength == 1) {
//     int8_t n = *(int8_t*) _recv_DataBuffer;
//     return (int32_t)n;
//   }else
//   if( _recv_DataLength == 2) {
//     int16_t n = *(int16_t*) _recv_DataBuffer;
//     return (int32_t)n;
//   }else
//   if( _recv_DataLength == 4) {
//     int32_t n = *(int32_t*) _recv_DataBuffer;
//     return (int32_t)n;
//   }else
//   if( _recv_DataLength == 8) {
//     int64_t n = *(int64_t*) _recv_DataBuffer;
//     return (int32_t)n;
//   }
// }

// int64_t BeeNeXT_Class::toInt64(){
//   if( _recv_DataLength == 1) {
//     int8_t n = *(int8_t*) _recv_DataBuffer;
//     return (int64_t)n;
//   }else
//   if( _recv_DataLength == 2) {
//     int16_t n = *(int16_t*) _recv_DataBuffer;
//     return (int64_t)n;
//   }else
//   if( _recv_DataLength == 4) {
//     int32_t n = *(int32_t*) _recv_DataBuffer;
//     return (int64_t)n;
//   }else
//   if( _recv_DataLength == 8) {
//     int64_t n = *(int64_t*) _recv_DataBuffer;
//     return (int64_t)n;
//   }
// }

size_t BeeNeXT_Class::value(uint8_t* value, uint16_t valueLength) {
  size_t lengthToCopy = min(_recv_DataLength, valueLength);
  memcpy(value, _recv_DataBuffer, lengthToCopy);
  return lengthToCopy;
}