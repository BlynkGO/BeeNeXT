#include <BeeNeXT.h>
#include "BeeBluetooth.h"

#if BEENEXT_USE_BEEBLUETOOTH

#if defined(ESP32)

#define BEE_MASTER_STR    "BeeNeXT_TFT" // "MASTER"
#define BEE_SLAVE_STR     "MCU" // "SLAVE"

BluetoothSerial * _bt_serial=NULL;
void(*_func_bt_recv_cb)(String key, String value);
bool _is_master = false;
BluetoothSerial _SerialBT;
bool _is_connected=false;

  
String beebluetooth::_mcu_address;

void beebluetooth::hostAddress(String address){
  beebluetooth::_mcu_address = address.c_str();
}

String beebluetooth::address(){
  if(_bt_serial==NULL) return "";
  return _bt_serial->getBtAddressString();
}

void beebluetooth::_init(bool is_master, void(*fn)(String key, String value)){
  _func_bt_recv_cb=fn;
  _bt_serial = &_SerialBT;
  _is_master = is_master;

  if( _bt_serial->begin( _is_master? "BT_" BEE_MASTER_STR  : "BT_" BEE_SLAVE_STR, _is_master)){
    Serial.printf("[BeeBluetooth] %s inited.     MAC: %s\n", 
      _is_master? BEE_MASTER_STR : BEE_SLAVE_STR, 
      _bt_serial->getBtAddressString().c_str());
    
    // สำหรับ BT Slave หากมี BT Master เข้ามาเชื่อมต่อ เช็ค address ของ Master ที่เข้ามาเชื่อมต่อ
    if( !_is_master ) {
      beebluetooth::_mcu_address = "";
      _bt_serial->register_callback([](esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
        switch(event){
          case ESP_SPP_SRV_OPEN_EVT: {
            _is_connected = true;
            Serial.print("[BeeBluetooth] client connected! MAC: ");
            for(int i=0;i<6;i++){
              Serial.printf("%02X", param->srv_open.rem_bda[i]);
              if(i<5) Serial.print(":");
            }
            Serial.println();
            break;
          }
          case ESP_SPP_CLOSE_EVT: {
            _is_connected = false;
            Serial.println("[BeeBluetooth] client disconnected!");
            break;
          }
        }
      });
    }else{
      // หากเป็น BT master
      static SoftTimer timer_connection;
      timer_connection.setInterval(1000,[](){
        if( _bt_serial->connected()) return;
        if(_is_connected){
          _is_connected = false;
          Serial.println("[BeeBluetooth] disconnected");
        }
        if( beebluetooth::_mcu_address != "") {
          BTAddress slaveAddress(beebluetooth::_mcu_address);
          if(_bt_serial->connect(slaveAddress)){
            _is_connected = true;
            Serial.printf("[BeeBluetooth] connected to : %s\n", beebluetooth::_mcu_address.c_str()); 
          }
        }
      });
    }
  }else{
    Serial.println("[BeeBluetooth] failed");
    return;
  }
  
  static SoftTimer timer;
  timer.setInterval(1,[](){
    if(_bt_serial != NULL) {
      if(_bt_serial->available()){
        String raw = _bt_serial->readStringUntil('\n'); raw.trim();
        if(raw.startsWith("[BN]")) {
          raw.replace("[BN]", "");
          if(raw.indexOf(":")> 0) {
            String key    = raw.substring(0, raw.indexOf(":"));
            String value  = raw.substring(raw.indexOf(":") + 1);
            if(_func_bt_recv_cb) _func_bt_recv_cb(key, value);
          }
        }
      }
    }
  });
}

// สำหรับ MCU
void beebluetooth::init(void(*fn)(String key, String value)) {
  beebluetooth::_init(BEE_SLAVE, fn);
}

// สำหรับ จอ BeeNeXT
void beebluetooth::init(String MCU_Address, void(*fn)(String key, String value))
{
  beebluetooth::_mcu_address = MCU_Address;
  beebluetooth::_init(BEE_MASTER, fn);
}

bool beebluetooth::connected(){
  return _is_connected;
}

void beebluetooth::print(String key, String value){
  if(_bt_serial !=NULL && _is_connected) {
    _bt_serial->println(String("[BN]")+ key + ":"+ value);
  }
}

void beebluetooth::print(String key, float value, int decimalPlaces){
  beebluetooth::print(key, String(value, decimalPlaces));
}

void beebluetooth::print(String key, double value, int decimalPlaces){
  beebluetooth::print(key, String(value, decimalPlaces));
}

void beebluetooth::print(String key, int value){
  beebluetooth::print(key, String(value));
}

#endif //#if defined(ESP32)
#endif //#if BEENEXT_USE_BEEBLUETOOTH
