#include <BeeNeXT.h>

// วิธีต่อ MCU หลังบ้าน แบบ BeeI2C กับจอ BeeNeXT
// UNO  ต่อขา SDA A4, SCL A5, GND ร่วม
// Nano ต่อขา SDA A4, SCL A5, GND ร่วม
// MEGA ต่อขา SDA 20, SCL 21, GND ร่วม
//
// ESP8266 ต่อขา SDA 4, SCL 5, GND ร่วม  หรือ กำหนดขาเองได้
// NodeMCU ต่อขา SDA D2, SCL D1, GND ร่วม หรือ กำหนดขาเองได้
// ESP32 ต่อขา SDA 21, SCL 22, GND ร่วม   หรือ กำหนดขาเองได้

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
    LCD.print("TEMP" , (float) random(100.0)/10.0 );    // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "TEMP"  และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
    LCD.print("HUMID", (float) random(100.0)/10.0 );    // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "HUMID" และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
  });  
}

void loop() {
  BeeNeXT.update();
}
