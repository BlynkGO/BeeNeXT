#include <BeeNeXT.h>

#define LED_PIN    13

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("BeeNeXT Test");

  // --- แบบใช้ Software Serial ----------
  BeeNeXT.begin( 6 ,  7 );  // BeeNeXT เริ่มทำงานด้วย Soft Serial ขา RX6, TX7

  pinMode(LED_PIN,OUTPUT);

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    // คำสั่ง BeeNeXT.send( key, value); สำหรับ ส่งค่า จาก MCU ปัจจุบัน ไปยังอีก MCU
    BeeNeXT.send("TEMP" , random(100.0)/10.0 );    // ส่งไปยังอีก MCU ด้วย key "TEMP"  และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
    BeeNeXT.send("HUMID", random(100.0)/10.0 );    // ส่งไปยังอีก MCU ด้วย key "HUMID" และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
  });

}

void loop() {
  BeeNeXT.update();
}

