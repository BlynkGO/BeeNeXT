// beenext_config.h กำหนดดังนี้
//       #define BEENEXT_USE_BEEI2C              0
//       #define BEENEXT_USE_BEEUART             1
//       #define BEENEXT_USE_BEEMQTT             0

#include <BlynkGOv3.h>

GGaugeRainbow gauge;
GLed  led;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  /*************************************
   * ใช้ Serial เป็น ขาต่อ ไปยัง MCU หลัก (ใช้ร่วม Serial Monitor)
   *********************/  
  BeeUART.onData([](String key, String value){
    Serial.println(key + " ---> " + value);
    if(key.startsWith("TEMP")){
      float temp = value.toFloat();
      gauge = temp;
    }else
    if(key.startsWith("HUMID")){
      //....
    }
    
  });

  /*************************************
   * ใช้ Serial2 เป็น ขาต่อ ไปยัง MCU หลัก (ใช้ร่วม Serial Monitor)
   *********************/  
  // BeeUART.onData(17,18, [](String key, String value){  // เช่นจอ BeeNeXT4.3, 5.0, 7.0
  //   Serial.println(key + " ---> " + value);
  //   if(key.startsWith("TEMP")){
  //     float temp = value.toFloat();
  //     gauge = temp;
  //   }else
  //   if(key.startsWith("HUMID")){
  //     //....
  //   }
  // });


  gauge.color(TFT_BLUE, TFT_RED);
  gauge.decimal(2);

  led.size(80,80);
  led.color(TFT_BLUE);
  led.clickable(true);
  led.onClicked([](GWIDGET){
    led.toggle();
    BeeUART.print("LED", led.isON());  // ส่ง key "LED" ระบุสถานะ ไปยัง MCU หน้าบ้าน
  });

}

void loop(){
  BlynkGO.update();
}
