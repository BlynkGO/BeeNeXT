// beenext_config.h กำหนดดังนี้
//       #define BEENEXT_USE_BEEI2C              0
//       #define BEENEXT_USE_BEEUART             0
//       #define BEENEXT_USE_BEEMQTT             0
//       #define BEENEXT_USE_BEEBLUETOOTH        1   // for ESP32 classic เท่านั้น
//
// MCU_ADDRESS  ให้นำค่าของ MCU Address จากโคดฝั่ง MCU_backend_code มาใช้ในการเชื่อมต่อ
//

#include <BlynkGOv3.h>

#define MCU_ADDRESS "C0:49:EF:68:C1:EE"

GGaugeRainbow gauge;
GLed  led;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  BeeBluetooth.onData([](String key, String value){
    Serial.println(key + " ---> " + value);
    if(key.startsWith("TEMP")){
      float temp = value.toFloat();
      gauge = temp;
    }else
    if(key.startsWith("HUMID")){
      //....
    }
    
  });


  gauge.color(TFT_BLUE, TFT_RED);
  gauge.decimal(2);

  led.size(80,80);
  led.color(TFT_BLUE);
  led.clickable(true);
  led.onClicked([](GWIDGET){
    led.toggle();
    BeeBluetooth.print("LED", led.isON());  // ส่ง key "LED" ระบุสถานะ ไปยัง MCU หน้าบ้าน
  });

}

void loop(){
  BlynkGO.update();
}
