#include <BeeNeXT.h>

#define LED_PIN    13

void setup() {
  Serial.begin(9600); Serial.println();
  Serial.println("BeeNeXT Test");

  // --- แบบใช้ Hardware Serial ----------
  // BeeNeXT.begin(&Serial);                  // BeeNeXT เริ่มทำงานด้วย Serial

  // --- แบบใช้ Hardware Serial2 ---------
  // Serial2.begin(9600, SERIAL_8N1, 16,17);  // Serial2 ของ ESP32
  // Serial2.begin(9600);                     // Serial2 ของ Arduino MEGA ขา RX16, TX17
  // Serial2.begin(9600);                     // Serial2 ของ STM32F1C8 ขา PA3 (RX2) และ  PA2 (TX2)
  // BeeNeXT.begin(&Serial2);                 // BeeNeXT เริ่มทำงานด้วย Serial2

  // --- แบบใช้ Software Serial ----------
  BeeNeXT.begin(/*RX : Yellow*/ 6 , /* TX : Blue*/ 7 );  // BeeNeXT เริ่มทำงานด้วย Soft Serial ขา RX6, TX7

  pinMode(LED_PIN,OUTPUT);

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    static uint32_t c;
    BeeNeXT.send("MY_NUM", c++);    // ตัวอย่างส่งแบบ key & value จาก MCU ปัจจุบันไปอีก MCU
  });

}

void loop() {
  BeeNeXT.update();
}

// ตัวอย่าง รับค่า value ที่ส่งมาจาก MCU อื่น ด้วย key "LED" 
// แล้วนำค่า value มากำหนดให้ led ที่ต่อกับ MCU นี้

BEENEXT_DATA(){
  if(BeeNeXT.key() == "LED") {            // เช็คว่า key ที่ส่งมาตรงกับ "LED" หรือไม่
    bool value = (bool) BeeNeXT.toInt();  // เปลี่ยนให้ value ที่ส่งมา ไปเป็นข้อมูลแบบ boolean  ; อาจจะเปลี่ยนไปค่าแบบต่างๆได้เช่น BeeNeXT.toString(), BeeNeXT.toInt(), BeeNeXT.toFloat(), BeeNeXT.toBool()  ได้ตามต้องการ
    digitalWrite(LED_PIN, value); 
  }
}



