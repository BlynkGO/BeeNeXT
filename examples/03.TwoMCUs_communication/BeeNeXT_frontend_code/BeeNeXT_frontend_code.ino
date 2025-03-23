#include <BlynkGOv3.h>

GGaugeRainbow gauge;
GLed  led;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  BeeI2C.onData([](String key, String value){
    Serial.println(key + " ---> " + value);
    if(key.startsWith("TEMP")){
      float temp = value.toFloat();
      gauge = temp;
    }
  });

  gauge.color(TFT_BLUE, TFT_RED);
  gauge.decimal(2);

  led.size(80,80);
  led.color(TFT_BLUE);
  led.clickable(true);
  led.onClicked([](GWIDGET){
    led.toggle();
    BeeI2C.print("LED", led.isON());  // ส่ง key "LED" ระบุสถานะ ไปยัง MCU หน้าบ้าน
  });

}

void loop(){
  BlynkGO.update();
}