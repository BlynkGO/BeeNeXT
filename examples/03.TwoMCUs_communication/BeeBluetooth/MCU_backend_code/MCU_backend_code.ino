// beenext_config.h กำหนดดังนี้
//       #define BEENEXT_USE_BEEI2C              0
//       #define BEENEXT_USE_BEEUART             0
//       #define BEENEXT_USE_BEEMQTT             0
//       #define BEENEXT_USE_BEEBLUETOOTH        1   // for ESP32 classic เท่านั้น
//
// หลัง init ให้นำ MAC Address ของ MCU ไปให้ฝั่ง จอ BeeNeXT_frontend_code ใช้ในการเชื่อมต่อ
//

#include <BeeNeXT.h>

#define LED_PIN     2
#define LED_ON      HIGH
#define LED_OFF     LOW

void setup() {
  Serial.begin(115200); Serial.println();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  LCD.begin([](String key, String value){
    Serial.println(key + " ---> " + value);
    if(key.startsWith("LED")) {
      bool led_state = value.toInt();
      digitalWrite(LED_PIN, led_state? LED_ON : LED_OFF);
    }
  });

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    float temp  = (float) random(200, 500)/10.0;  // ของจริงให้อ่านค่าจาก sensor
    float humid = (float) random(600, 900)/10.0;  // ของจริงให้อ่านค่าจาก sensor
    Serial.printf("[TEMP] %.1f ; [HUMID] %.1f\n", temp, humid);
    LCD.print("TEMP" , temp );    // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "TEMP"  และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
    LCD.print("HUMID", humid );    // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "HUMID" และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
  });  
}

void loop() {
  BeeNeXT.update();
}
