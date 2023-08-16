#include "BeeNeXT.h"

// #if defined(BEENEXT) || BLYNKGO_USE_BEENEXT

void BeeNeXT_NoOpCbk() {}
BEENEXT_DATA()          __attribute__((weak, alias("BeeNeXT_NoOpCbk")));
SERIAL_DATA()           __attribute__((weak, alias("BeeNeXT_NoOpCbk")));
BEENEXT_CONNECTED()     __attribute__((weak, alias("BeeNeXT_NoOpCbk")));
BEENEXT_DISCONNECTED()  __attribute__((weak, alias("BeeNeXT_NoOpCbk")));

BeeNeXT_class BeeNeXT;
static SoftTimer timer_delay;

bool BeeNeXT_class::_beenext_enable = true;

BeeNeXT_class::~BeeNeXT_class(){
  this->end();
}

#if CONFIG_IDF_TARGET_ESP32C3
void BeeNeXT_class::begin(HardwareSerial *serial){
  // ยังไม่ได้ impliment
}

#else
void BeeNeXT_class::begin(HardwareSerial *serial){
  this->end();    // serial ของเดิม ไม่ว่าจะ hard serial หรือ soft serial ที่ใช้อยู่เดิม ให้ ยกเลิกไปก่อน

  Serial.println("[BeeNeXT] HardwareSerial");
#if defined(__STM32F1__)
  _hw_serial = serial;
#elif CONFIG_IDF_TARGET_ESP32S3
  #if defined(BEENEXT_7_0IPS)
    _hw_serial = serial;
  #else
    #if defined(BEENEXT_4_3) ||  defined(BEENEXT_4_3C) ||  defined(BEENEXT_4_3IPS)
      pinMode(18,OUTPUT);
      digitalWrite(18, LOW); // ทำขา 18 เป็น GND
    #endif // #if defined(BEENEXT_4_3) ||  defined(BEENEXT_4_3C) ||  defined(BEENEXT_4_3IPS)
    Serial.println("[BeeNeXT] ESP32S3 begin on Serial2 (19,20)");
    Serial2.begin(9600);  // RX19 ; TX20
    _hw_serial = &Serial2;
  #endif
#else  // ESP32 classic
  _hw_serial = (serial == NULL)? &Serial : serial;  // ไม่มีการ serial begin() มาก่อนเอาเอง

  #if defined(ESP8266)
    Serial.println("[BeeNeXT] ESP8266 begin on HardwareSerial");
  #elif defined(ESP32)
    Serial.printf("[BeeNeXT] ESP32 begin on %s\n", (_hw_serial==&Serial)? "Serial" : (_hw_serial==&Serial2)? "Serial2" : "Unknown Serial");
  #endif

#endif // #if CONFIG_IDF_TARGET_ESP32S3

  _hw_serial->setTimeout(50);
  _hw_serial->flush();
  this->set_heartbeat(1000);
  this->set_heartbeat_checker();

#if defined(ESP8266) || defined(ESP32)
  timer_delay.delay(1000,[](){ if(!BeeNeXT.connected()) BeeNeXT_onDisconnected(); });  
#else
  delay(1000);
  if(!BeeNeXT.connected()) BeeNeXT_onDisconnected();
#endif
}
#endif

#if defined(__STM32F1__)
void BeeNeXT_class::begin(USBSerial* serial){
  _usb_serial = serial;
  _usb_serial->setTimeout(50);
  _usb_serial->flush();
  this->set_heartbeat(1000);
  this->set_heartbeat_checker();
}
#endif  // #if defined(__STM32F1__)

#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
void BeeNeXT_class::begin(HardwareSerial &serial ){
  this->begin(&serial);
}
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
void BeeNeXT_class::begin(unsigned long baud, uint8_t rx, uint8_t tx){
  this->end();
  if(_sw_serial==NULL)
    _sw_serial =  new SoftwareSerial();
  if(_sw_serial != NULL) {
    _is_swserial_alloced = true;
    _sw_serial->begin(baud, rx, tx);
    _sw_serial->setTimeout(50);
    _sw_serial->flush();
  }
  this->set_heartbeat(1000);
  this->set_heartbeat_checker();

#if defined(ESP8266) || defined(ESP32)
  timer_delay.delay(1000,[](){ if(!BeeNeXT.connected()) BeeNeXT_onDisconnected(); });  
#else
  delay(1000);
  if(!BeeNeXT.connected()) BeeNeXT_onDisconnected();
#endif
 
}

void BeeNeXT_class::begin(uint8_t rx, uint8_t tx){
  this->begin(9600, rx,tx);
}

void BeeNeXT_class::begin(SoftwareSerial *softserial) {
  this->end();
  Serial.println("[BeeNeXT] SoftwareSerial");
  _sw_serial = softserial;
  _sw_serial->setTimeout(50);
}

void BeeNeXT_class::begin(SoftwareSerial &softserial){
  this->begin(&softserial);
}

#endif //#if BEENEXT_USE_SOFTWARESERIAL

// #if defined(CONFIG_IDF_TARGET_ESP32S3)
// void BeeNeXT_class::begin(unsigned long baud, uint8_t rx, uint8_t tx){
//   Serial.println("[BeeNeXT] use Serial2");
//   Serial2.begin(baud, SERIAL_8N1, rx, tx);
//   this->begin(&Serial2);
// }

// void BeeNeXT_class::begin(uint8_t rx, uint8_t tx){
//   this->begin(115200, rx, tx);
// }
// #endif // #if defined(ESP32)



void BeeNeXT_class::end(){
  if( _hw_serial != NULL) { 
    _hw_serial->end(); 
    _hw_serial = NULL;
  }

#if defined(__STM32F1__)
  if(_usb_serial!= NULL) { 
    _usb_serial->end(); 
    _usb_serial = NULL;
  }
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  #if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
  if( _sw_serial != NULL ){
    _sw_serial->end();
    if(_is_swserial_alloced){
      free(_sw_serial); _sw_serial = NULL;
      _is_swserial_alloced = false;
    }
  }
  #endif
#endif
}

// void  BeeNeXT_class::extract_key_value(){
//   int index = _data.indexOf(":");
//   if( index > 0) {
//     _key   = _data.substring(0, index);
//     _value = _data.substring(index+1);
//   }else{
//     _key   = "";
//     _value = "";
//   }
// }



void BeeNeXT_class::set_heartbeat(uint32_t heartbeat_interval){
  _timer_heartbeat.setInterval(heartbeat_interval,[](){
    if(BeeNeXT_class::_beenext_enable) BeeNeXT.send("_bhb_", true);
  });
}

void BeeNeXT_class::set_heartbeat_checker(){
  _timer_heartbeat_checker.setInterval(BEENEXT_CONNECTION_TIMEOUT,[](){
    if(millis()> BeeNeXT._millis_heartbeat + BEENEXT_CONNECTION_TIMEOUT){
      if(BeeNeXT._bee_connected) {
        BeeNeXT._bee_connected = false;
        BeeNeXT_onDisconnected();
        BeeNeXT.set_heartbeat(1000);
      }
    }
  }, true);
}

void BeeNeXT_class::enable(bool en){
  BeeNeXT_class::_beenext_enable = !!en;
  if(BeeNeXT_class::_beenext_enable) {
    Serial.println("[BeeNeXT] enable");
  }else{
    Serial.println("[BeeNeXT] disable");    
  }
}
void BeeNeXT_class::_extract_key_value(char ch){
  switch (_receiveState) {
  case BEENEXT_WAIT_FOR_PREHEADER:
    if (ch == '[') { // เมื่อตัวแรกของ pre-header เป็น '['
      _receivedPreHeader[0]   = ch; // เก็บตัวอักษรแรกของ pre-header ไว้
      _receivedPreHeaderChars = 1;  // เริ่มต้นนับตัวอักษรแรกของ pre-header
      _receiveState = BEENEXT_WAIT_FOR_PREHEADER_CHECK;
    } else {
      // ถ้าไม่ใช่ '[' ให้เริ่มต้นการรับ pre-header ใหม่
      _receivedPreHeaderChars = 0;
    }
    break;
  case BEENEXT_WAIT_FOR_PREHEADER_CHECK:
    _receivedPreHeader[_receivedPreHeaderChars] = ch; // เก็บตัวอักษรถัดไปของ pre-header ไว้
    _receivedPreHeaderChars++;
    if (_receivedPreHeaderChars == sizeof(preHeader) - 1) {
      if (memcmp(_receivedPreHeader, preHeader, sizeof(preHeader) - 1) == 0) {
        // pre-header ถูกต้อง
        _receiveState = BEENEXT_WAIT_FOR_KEY_LENGTH;
      } else {
        // pre-header ไม่ถูกต้อง
        // ทำการตรวจสอบและหากเกิดข้อผิดพลาดที่นี่
        // ...
        // เมื่อประมวลผลเสร็จสิ้น กลับไปรอรับข้อมูลใหม่
        _receiveState = BEENEXT_WAIT_FOR_PREHEADER;
      }
      _receivedPreHeaderChars = 0; // รีเซ็ตการนับตัวอักษร pre-header ใหม่
    }
    break;
  case BEENEXT_WAIT_FOR_KEY_LENGTH:
    _receivedKeyLength = ch;
    _receivedDataLengthBytes = 0;
    _receiveState = BEENEXT_WAIT_FOR_DATA_LENGTH;
    break;
  case BEENEXT_WAIT_FOR_DATA_LENGTH:
    if (_receivedDataLengthBytes == 0) {
      ((uint8_t *)&_receivedDataLength)[0] = ch;
      _receivedDataLengthBytes = 1;
    } else {
      ((uint8_t *)&_receivedDataLength)[1] = ch;
      _receivedKeyChars = 0;
      _receiveState = BEENEXT_WAIT_FOR_KEY;
    }
    break;
  case BEENEXT_WAIT_FOR_KEY:
    _receivedKeyBuffer[_receivedKeyChars] = ch;
    _receivedKeyChars++;
    if (_receivedKeyChars == _receivedKeyLength) {
      _receivedDataChars = 0;
      _receiveState = BEENEXT_WAIT_FOR_DATA;
    }
    break;
  case BEENEXT_WAIT_FOR_DATA:
    _receivedDataBuffer[_receivedDataChars] = ch;
    _receivedDataChars++;
    if (_receivedDataChars == _receivedDataLength) {
      for(int i=0;i< _receivedDataLength; i++){
        Serial.println(_receivedDataBuffer[i], HEX);
      }
      _receiveState = BEENEXT_WAIT_FOR_CHECKSUM;
    }
    break;
  case BEENEXT_WAIT_FOR_CHECKSUM:
    _receivedChecksum = ch;
    
    // คำนวณ checksum ของ pre-header
    uint8_t _calculatedChecksum = 0;
    for (size_t i = 0; i < sizeof(preHeader) - 1; i++) {
      _calculatedChecksum ^= preHeader[i];
    }
    _calculatedChecksum ^= _receivedKeyLength;
    _calculatedChecksum ^= ((uint8_t *)&_receivedDataLength)[0];
    _calculatedChecksum ^= ((uint8_t *)&_receivedDataLength)[1];
    for (size_t i = 0; i < _receivedKeyLength; i++) {
      _calculatedChecksum ^= _receivedKeyBuffer[i];
    }
    for (size_t i = 0; i < _receivedDataLength; i++) {
      _calculatedChecksum ^= _receivedDataBuffer[i];
    }
    Serial.print("[recv checksum] ");
    Serial.println(_receivedChecksum);
    Serial.print("[calc checksum] ");
    Serial.println(_calculatedChecksum);
    
//            uint8_t _calculatedChecksum = _calculateChecksum(_receivedDataBuffer, _receivedDataLength);
    if (_receivedChecksum == _calculatedChecksum) {
      // Checksum ถูกต้อง
      // เรียกใช้งาน callback function ถ้ามี callback function ถูกกำหนดไว้
      if (callback) {
        callback();
      }
    } else {
      // Checksum ไม่ถูกต้อง
    }
    // เมื่อประมวลผลเสร็จสิ้น กลับไปรอรับข้อมูลใหม่
    _receiveState = BEENEXT_WAIT_FOR_PREHEADER;
    break;
  }
}

void BeeNeXT_class::update(){
#if BEENEXT_USE_SOFTTIMER
  SoftTimer::run();
#endif

  if(!BeeNeXT_class::_beenext_enable) return;


  if(_hw_serial != NULL) {
    if(_hw_serial->available()){
      while (_hw_serial->available() > 0) {
        char ch = _hw_serial->read();
        this->_extract_key_value(ch);
      }
    }
  }

#if defined(__STM32F1__)
  else if(_usb_serial != NULL) {
    if(_usb_serial->available()){
      while (_usb_serial->available() > 0) {
        char ch = _usb_serial->read();
        this->_extract_key_value(ch);
      }
    }
  }
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  else if(_sw_serial !=NULL ){
    if(_sw_serial->available()){
      // Serial.println("[BeeNeXT] available on SoftwareSerial");
#if defined(ESP8266) || defined(ESP32)
#else
      _sw_serial->flush();  // ? จำเป็นไหม สำหรับ MCU ที่ไม่ใช่ ESP8266/ESP32
#endif
      while (_sw_serial->available() > 0) {
        char ch = _sw_serial->read();
        this->_extract_key_value(ch);
      }
    }
  }
#endif

//     if(_hw_serial->available()){
//       // Serial.println("[BeeNeXT] available on HardwareSerial");
// #if defined(ESP8266) || defined(ESP32)
// #else
//       _hw_serial->flush();
// #endif
//       String data = _hw_serial->readStringUntil('\n'); _data.trim();
//       // Serial.print("[1] ");
//       // Serial.print("[BeeNeXT] recv : "); Serial.println(data);
//       if(data.startsWith("[BN]")){
//         data.replace("[BN]", "");
//         _data = data;
//         this->extract_key_value();
//         // Serial.printf("[_key] : %s\n", _key.c_str());
//         if( _key == "_bhb_"){
//           // Serial.println("[_bhb_] found");
//           _millis_heartbeat = millis();
//           if( (bool) _value.toInt() != _bee_connected ) {
//             _bee_connected = _value.toInt();
//             if(_bee_connected ) {
//               BeeNeXT_onConnected();
//               this->set_heartbeat(BEENEXT_CONNECTION_TIMEOUT);
//             }else {
//               BeeNeXT_onDisconnected();
//               this->set_heartbeat(1000);
//             }
//           }
//         }else{
//           BeeNeXT_onData();
//         }
//         _data  = "";
//         _key   = "";
//         _value = "";
//       }else{
//         _data = data;
//         BeeNeXT_onSerialData();
//         _data  = "";
//         _key   = "";
//         _value = "";
//       }
//     }
//  }
// #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
//   else if(_sw_serial !=NULL ){
//     if(_sw_serial->available()){
//       // Serial.println("[BeeNeXT] available on SoftwareSerial");
// #if defined(ESP8266) || defined(ESP32)
// #else
//       _sw_serial->flush();
// #endif
//       String data = _sw_serial->readStringUntil('\n'); _data.trim();
//       // Serial.print("[2] ");
//       // Serial.println(data);
//       if(data.startsWith("[BN]")){
//         data.replace("[BN]", "");
//         _data = data;
//         this->extract_key_value();
//         if( _key == "_bhb_"){
//           _millis_heartbeat = millis();
//           if( (bool) _value.toInt() != _bee_connected ) {
//             _bee_connected = _value.toInt();
//             if(_bee_connected ) {
//               BeeNeXT_onConnected();
//               this->set_heartbeat(BEENEXT_CONNECTION_TIMEOUT);
//             }else {
//               BeeNeXT_onDisconnected();
//               this->set_heartbeat(1000);
//             }
//           }
//         }else{
//           BeeNeXT_onData();
//         }

//         _data  = "";
//         _key   = "";
//         _value = "";
//       }else{
//         _data = data;
//         BeeNeXT_onSerialData();
//         _data  = "";
//         _key   = "";
//         _value = "";
//       }
//     }
//   }
// #endif

}



// void BeeNeXT_class::command(uint16_t cmd) { 
//   if(cmd>=0 && cmd < CMD_MAX) {
//     String _cmd = String("cmd:")+String(cmd);
//     Serial.println(_cmd);
//     this->println(_cmd); 
//   }
// }

void BeeNeXT_class::event_send(beenect_event_t event){
  switch(event){
    case EVENT_BEENEXT_CONNECTED:
      BeeNeXT_onConnected();
      break;
    case EVENT_BEENEXT_DISCONNECTED:
      BeeNeXT_onDisconnected();
      break;
    case EVENT_BEENEXT_DATA:
      BeeNeXT_onData();
      break;
    case EVENT_SERIAL_DATA:
      BeeNeXT_onSerialData();
      break;
  }
}

void BeeNeXT_class::send(String key, uint8_t *data, uint16_t data_len) {
  // ส่ง pre-header
  if(_hw_serial != NULL) {
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
    _hw_serial->write(preHeader, sizeof(preHeader) - 1);
    // ส่งความยาวของ key
    uint8_t  keyLength = key.length();
    _hw_serial->write(keyLength);
    // ส่งความยาวของ data (2 byte)
    _hw_serial->write((uint8_t *)&data_len, 2);

    // ส่ง key
    _hw_serial->write(key.c_str(), keyLength);

    // ส่ง data
    _hw_serial->write(data, data_len);
#elif defined(__STM32F1__)
    _hw_serial->write((const uint8*)preHeader, sizeof(preHeader) - 1);
    // ส่งความยาวของ key
    uint8_t  keyLength = key.length();
    _hw_serial->write(keyLength);
    // ส่งความยาวของ data (2 byte)
    _hw_serial->write((const uint8*)&data_len, 2);

    // ส่ง key
    _hw_serial->write((const uint8*)key.c_str(), keyLength);

    // ส่ง data
    _hw_serial->write((const uint8*)data, data_len);
#endif

    // คำนวณและส่ง checksum ของทั้งหมด
    uint8_t checksum = 0;
    for (size_t i = 0; i < sizeof(preHeader) - 1; i++) {
      checksum ^= preHeader[i];
    }
    checksum ^= keyLength;
    checksum ^= ((uint8_t *)&data_len)[0];
    checksum ^= ((uint8_t *)&data_len)[1];
    for (size_t i = 0; i < keyLength; i++) {
      checksum ^= key[i];
    }
    for (size_t i = 0; i < data_len; i++) {
      checksum ^= data[i];
    }
    _hw_serial->write(checksum);
  }

#if defined(__STM32F1__)
  if(_usb_serial != NULL) {
    _usb_serial->write((const uint8*)preHeader, sizeof(preHeader) - 1);
    // ส่งความยาวของ key
    uint8_t  keyLength = key.length();
    _usb_serial->write(keyLength);
    // ส่งความยาวของ data (2 byte)
    _usb_serial->write((const uint8*)&data_len, 2);

    // ส่ง key
    _usb_serial->write((const uint8*)key.c_str(), keyLength);

    // ส่ง data
    _usb_serial->write((const uint8*)data, data_len);

    // คำนวณและส่ง checksum ของทั้งหมด
    uint8_t checksum = 0;
    for (size_t i = 0; i < sizeof(preHeader) - 1; i++) {
      checksum ^= preHeader[i];
    }
    checksum ^= keyLength;
    checksum ^= ((uint8_t *)&data_len)[0];
    checksum ^= ((uint8_t *)&data_len)[1];
    for (size_t i = 0; i < keyLength; i++) {
      checksum ^= key[i];
    }
    for (size_t i = 0; i < data_len; i++) {
      checksum ^= data[i];
    }
    _usb_serial->write(checksum);
  }
#endif

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  else if(_sw_serial !=NULL ){
    _sw_serial->write(preHeader, sizeof(preHeader) - 1);
    // ส่งความยาวของ key
    uint8_t  keyLength = key.length();
    _sw_serial->write(keyLength);
    // ส่งความยาวของ data (2 byte)
    _sw_serial->write((uint8_t *)&data_len, 2);

    // ส่ง key
    _sw_serial->write(key.c_str(), keyLength);

    // ส่ง data
    _sw_serial->write(data, data_len);


    // คำนวณและส่ง checksum ของทั้งหมด
    uint8_t checksum = 0;
    for (size_t i = 0; i < sizeof(preHeader) - 1; i++) {
      checksum ^= preHeader[i];
    }
    checksum ^= keyLength;
    checksum ^= ((uint8_t *)&data_len)[0];
    checksum ^= ((uint8_t *)&data_len)[1];
    for (size_t i = 0; i < keyLength; i++) {
      checksum ^= key[i];
    }
    for (size_t i = 0; i < data_len; i++) {
      checksum ^= data[i];
    }
    _sw_serial->write(checksum);
  }
#endif

  // รอสักครู่
  delay(1);
}

// void BeeNeXT_class::send(String key, String value){
//   if( _hw_serial != NULL) { 
//     _hw_serial->println(String("[BN]") + key+":"+value);
//   }
// #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
//   if( _sw_serial != NULL ){
//     _sw_serial->println(String("[BN]") + key+":"+value);
//   }
// #endif
// }

// void BeeNeXT_class::send(String key, uint8_t* data, size_t data_len){
//   if( _hw_serial != NULL) { 
//     _hw_serial->print(String("[BN]") + key+":");
//     _hw_serial->write(data, data_len);
//     _hw_serial->println();
//   }
// #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
//   if( _sw_serial != NULL ){
//     _sw_serial->print(String("[BN]") + key+":");
//     _sw_serial->write(data, data_len);
//     _sw_serial->println();
//   }
// #endif
// }
//------------------------------------------------------------
//virtual function
// size_t BeeNeXT_class::write(uint8_t data){
//   if(_hw_serial){
//     if(!this->isConnected()) return 0;
//     _hw_serial->write('[');_hw_serial->write('B');_hw_serial->write('N');_hw_serial->write(']');
//     size_t sz = _hw_serial->write(data);
//     return sz+4;
//   }
// #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
//   else if( _sw_serial){
//     _sw_serial->write('[');_sw_serial->write('B');_sw_serial->write('N');_sw_serial->write(']');
//     size_t sz = _sw_serial->write(data);
//     return sz+4;
//   }
// #endif
//   return 0;
// }

// size_t BeeNeXT_class::write(const uint8_t *buffer, size_t size){
//   if(_hw_serial){
//     if(!this->isConnected()) return 0;
//     // Serial.println("[11]");
//     _hw_serial->write('[');_hw_serial->write('B');_hw_serial->write('N');_hw_serial->write(']');
//     size_t sz = _hw_serial->write(buffer, size);
//     return sz + 4;
//   }
// #if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
//   else if(_sw_serial){
//     // Serial.printf("[BeeNeXT] SoftSerial printf : [BN]%.*s", size, buffer);
//     // Serial.println("[22]");
//     _sw_serial->write('[');_sw_serial->write('B');_sw_serial->write('N');_sw_serial->write(']');
//     size_t sz = _sw_serial->write(buffer, size);
//     return sz + 4;
//   }
// #endif
//   return 0;
// }


// #endif //#if defined(BEENEXT) || BLYNKGO_USE_BEENEXT
