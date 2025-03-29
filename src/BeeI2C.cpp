#include "BeeI2C.h"

#if BEENEXT_USE_BEEI2C

#include <BeeNeXT.h>
#include <vector>

#define WIRE_MASTER_BUS      Wire
#define BEEI2C_ADDR   0x55
#define I2C_REQ_SIZE  255

void(*_func_i2c_recv_cb)(String key, String value);
void McuI2C_Master::init(void(*fn)(String key, String value)){
  _func_i2c_recv_cb = fn;
  WIRE_MASTER_BUS.begin();
  Serial.println("[BeeI2C] on 'I2C Master'");
  static SoftTimer timer;
  timer.setInterval(1,[](){
    size_t sz = WIRE_MASTER_BUS.requestFrom(0x55, I2C_REQ_SIZE);
    if(sz > 5) {
      if( WIRE_MASTER_BUS.available()){
        String raw = WIRE_MASTER_BUS.readStringUntil('\n'); raw.trim();
        if(raw.indexOf(":")> 0) {
          String key    = raw.substring(0, raw.indexOf(":"));
          String value  = raw.substring(raw.indexOf(":") + 1);
          if(_func_i2c_recv_cb) _func_i2c_recv_cb(key, value);
        }
      }
    }
  });
}

#if  defined(ESP8266) || defined(ESP32)
void McuI2C_Master::init(uint8_t sda, uint8_t scl, void(*fn)(String key, String value)){
  _func_i2c_recv_cb = fn;
  WIRE_MASTER_BUS.begin(sda, scl);
  Serial.printf("[BeeI2C] on 'I2C Master' (sda %d, scl %d)\n", sda, scl);
  static SoftTimer timer;
  timer.setInterval(1,[](){
    size_t sz = WIRE_MASTER_BUS.requestFrom(0x55, I2C_REQ_SIZE);
    if(sz > 5) {
      if( WIRE_MASTER_BUS.available()){
        String raw = WIRE_MASTER_BUS.readStringUntil('\n'); raw.trim();
        if(raw.indexOf(":")> 0) {
          String key    = raw.substring(0, raw.indexOf(":"));
          String value  = raw.substring(raw.indexOf(":") + 1);
          if(_func_i2c_recv_cb) _func_i2c_recv_cb(key, value);
        }
      }
    }
  });
}
#endif


void McuI2C_Master::print(String key, String value){
  WIRE_MASTER_BUS.beginTransmission(BEEI2C_ADDR);
  WIRE_MASTER_BUS.println(key+":"+value);
  WIRE_MASTER_BUS.endTransmission();
}

void McuI2C_Master::print(String key, int value){
  McuI2C_Master::print(key, String(value));
}

void McuI2C_Master::print(String key, float value, int decimalPlaces){
  McuI2C_Master::print(key, String(value, decimalPlaces));
}

void McuI2C_Master::print(String key, double value, int decimalPlaces){
  McuI2C_Master::print(key, String(value, decimalPlaces));
}

//--------------------------------------------------------------------

#if defined(ESP32)

#define WIRE_SLAVE_BUS      Wire1

typedef struct {
  String key;
  String value;
} beei2c_data_t;

std::vector<beei2c_data_t> beei2c_vector;
// void(*_func_i2c_recv_cb)(String key, String value);

void BeeI2C_Slave::init(uint8_t sda, uint8_t scl,void(*fn)(String key, String value)){
  _func_i2c_recv_cb = fn;
  if(WIRE_SLAVE_BUS.begin(BEEI2C_ADDR, sda, scl, 100000)){
    Serial.println("[BeeI2C] init : OK");
  }else{
    Serial.println("[BeeI2C] init : Failed");    
  }
  WIRE_SLAVE_BUS.onRequest([](){
    if(beei2c_vector.size()){
      beei2c_data_t beei2c_data = beei2c_vector.front();
      beei2c_vector.erase(beei2c_vector.begin());
      WIRE_SLAVE_BUS.println( beei2c_data.key+ ":"+beei2c_data.value );
    }
  });

  WIRE_SLAVE_BUS.onReceive([](int num_bytes){
    if (WIRE_SLAVE_BUS.available()) {
      String raw = WIRE_SLAVE_BUS.readStringUntil('\n'); raw.trim();
//      Serial.println(raw);
      if(raw.indexOf(":")> 0) {
        String key    = raw.substring(0, raw.indexOf(":"));
        String value  = raw.substring(raw.indexOf(":") + 1);
        if(_func_i2c_recv_cb) _func_i2c_recv_cb(key, value);
      }
    }
  });
}



void BeeI2C_Slave::print(String key, String value){
  beei2c_data_t beei2c_data;
    beei2c_data.key   = key;
    beei2c_data.value = value;
  beei2c_vector.push_back(beei2c_data);
}

void BeeI2C_Slave::print(String key, int value){
  BeeI2C_Slave::print(key, String(value));
}

void BeeI2C_Slave::print(String key, float value, int decimalPlaces){
  BeeI2C_Slave::print(key, String(value, decimalPlaces));
}

void BeeI2C_Slave::print(String key, double value, int decimalPlaces){
  BeeI2C_Slave::print(key, String(value, decimalPlaces));
}

#endif // #if defined(ESP32)


#endif //#if BEENEXT_USE_BEEI2C