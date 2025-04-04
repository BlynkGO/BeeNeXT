// ติดตั้ง ไลบรารี่ PubSubClient ด้วย

// beenext_config.h กำหนดดังนี้
//       #define BEENEXT_USE_BEEI2C              0
//       #define BEENEXT_USE_BEEUART             0
//       #define BEENEXT_USE_BEEMQTT             1


#include <BeeNeXT.h>
#include "HWButton.h"

#define SSID          "-----------------"
#define PASSWORD      "-----------------"

#define MQTT_HOST     "broker.emqx.io"
#define MQTT_PORT     1883
#define MQTT_USER     ""
#define MQTT_PASS     ""


#define LED_PIN     2
#define LED_ON      HIGH
#define LED_OFF     LOW

HWButton btn(0);

void setup() {
  Serial.begin(115200); Serial.println();

  WiFi.begin(SSID, PASSWORD);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  btn.onClicked([](HWButton* b){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  });

  LCD.begin(MQTT_HOST, MQTT_PORT, MQTT_USER, MQTT_PASS, [](String topic, String message){
    // เมื่อได้รับค่า จากที่ subscribe ไปจาก MQTT Host
    Serial.println(topic+ " ---> "+ message);
    if(topic.startsWith("/myBeeNeXT/LED")){
      bool led_state = message.toInt();
      digitalWrite(LED_PIN, led_state? LED_ON : LED_OFF);
    }
  });
  LCD.subscribe("/myBeeNeXT/LED");         // subscribe ขอรับข้อมูลจาก topic ที่ต้องการ
  // LCD.subscribe("/mymcu/TEMP");
  // LCD.client_id("xxxxx");                    // แบบกำหนด client_id เอง เช่นใช้ใน NetPIE

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    float temp  = (float) random(20, 50)/10.0;  // ของจริงให้อ่านค่าจาก sensor
    float humid = (float) random(60, 90)/10.0;  // ของจริงให้อ่านค่าจาก sensor
    Serial.printf("[TEMP] %.1f ; [HUMID] %.1f\n", temp, humid);
    LCD.publish("/mymcu/TEMP" , temp);     // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "TEMP"  และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
    LCD.publish("/mymcu/HUMID", humid);    // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "HUMID" และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
  });  
}

void loop() {
  BeeNeXT.update();
}
