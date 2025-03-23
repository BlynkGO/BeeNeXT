/* 
platformio.ini เพิ่ม 2 บรรทัดนี้ เพื่อเปิดใช้ MQTT32 ภายใน
-------------------------------------
build_flags=
  -D BLYNKGO_USE_ASYNC_MQTT32=1
-------------------------------------
*/

#include <BlynkGOv3.h>

#define MQTT_HOST     "broker.emqx.io"
#define MQTT_PORT     1883
#define MQTT_USER     ""
#define MQTT_PASS     ""

GGaugeRainbow gauge;
GLed  led;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  MQTT.setServer( MQTT_HOST, MQTT_PORT, MQTT_USER, MQTT_PASSWORD );
  MQTT.onMessage([]( String topic, String message){
    Serial.println(topic+ " ---> "+ message);
    if(String.startsWith("/mymcu/TEMP")){
      float temp = message.toFloat();
      gauge = temp;
    }else
    if(String.startsWith("/mymcu/HUMID")){
      // ...
    }
  });
  MQTT.subscribe("/mymcu/TEMP");
  MQTT.subscribe("/mymcu/HUMID");

  gauge.color(TFT_BLUE, TFT_RED);
  gauge.decimal(2);

  led.size(80,80);
  led.color(TFT_BLUE);
  led.clickable(true);
  led.onClicked([](GWIDGET){
    led.toggle();
    MQTT.publish("/myBeeNeXT/LED", led.isON());  // ส่ง key "LED" ระบุสถานะ ไปยัง MCU หน้าบ้าน
  });

}

void loop(){
  BlynkGO.update();
}
