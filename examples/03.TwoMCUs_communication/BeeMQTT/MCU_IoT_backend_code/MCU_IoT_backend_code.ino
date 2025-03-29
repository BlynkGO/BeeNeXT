// ติดตั้ง ไลบรารี่ PubSubClient ด้วย

// beenext_config.h กำหนดดังนี้
//       #define BEENEXT_USE_BEEI2C              0
//       #define BEENEXT_USE_BEEUART             0
//       #define BEENEXT_USE_BEEMQTT             1


#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif
#include <BeeNeXT.h>

#define SSID          "---------------"
#define PASSWORD      "---------------"

#define MQTT_HOST     "broker.emqx.io"
#define MQTT_PORT     1883
#define MQTT_USER     ""
#define MQTT_PASS     ""


#if defined(ESP8266) || defined(ESP32)
  #define LED_PIN     2
  #define LED_ON      LOW
  #define LED_OFF     HIGH
#else  // UNO, MEGA, NANO
  #define LED_PIN     13
  #define LED_ON      HIGH
  #define LED_OFF     LOW
#endif

void setup() {
  Serial.begin(115200); Serial.println();

  WiFi.begin(SSID, PASSWORD);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  LCD.begin(MQTT_HOST, MQTT_PORT, MQTT_USER, MQTT_PASS, [](String topic, String message){
    Serial.println(topic+ " ---> "+ message);
    if(topic.startsWith("/myBeeNeXT/LED")){
      bool led_state = message.toInt();
      digitalWrite(LED_PIN, led_state? LED_ON : LED_OFF);
    }
  });
  LCD.subscribe("/myBeeNeXT/LED");

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    LCD.publish("/mymcu/TEMP" , (float) random(100.0)/10.0 );    // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "TEMP"  และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
    LCD.publish("/mymcu/HUMID", (float) random(100.0)/10.0 );    // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "HUMID" และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
  });  
}

void loop() {
  BeeNeXT.update();
}
