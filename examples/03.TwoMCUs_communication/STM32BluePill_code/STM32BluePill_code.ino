#include <BeeNeXT.h>

#define LED_PIN    PC13

#define LED_ON      LOW
#define LED_OFF     HIGH

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("BeeNeXT Test");

  // --- แบบใช้ Serial2 ----------
  Serial2.begin(9600);    // STM32 BluePill Serial2 ขา PA3, PA2
  BeeNeXT.begin( &Serial2 );  

  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    // คำสั่ง BeeNeXT.send( key, value); สำหรับ ส่งค่า จาก MCU ปัจจุบัน ไปยังอีก MCU
    BeeNeXT.send("TEMP" , (float) random(100.0)/10.0 );    // ส่งไปยังอีก MCU ด้วย key "TEMP"  และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
    BeeNeXT.send("HUMID", (float) random(100.0)/10.0 );    // ส่งไปยังอีก MCU ด้วย key "HUMID" และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
  });

}

void loop() {
  BeeNeXT.update();
}
