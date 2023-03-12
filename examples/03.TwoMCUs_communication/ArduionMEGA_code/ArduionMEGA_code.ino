#include <BeeNeXT.h>

#define LED_PIN    13

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("BeeNeXT Test");

  // --- แบบใช้ Hardware Serial2 ---------
  Serial2.begin(9600);                     // Serial2 ของ Arduino MEGA ขา RX16, TX17
  BeeNeXT.begin(&Serial2);                 // BeeNeXT เริ่มทำงานด้วย Serial2


  pinMode(LED_PIN,OUTPUT);

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    BeeNeXT.send("TEMP" , random(100.0)/10.0 );    // ตัวอย่างส่งแบบ key & value จาก MCU ปัจจุบันไปอีก MCU
    BeeNeXT.send("HUMID", random(100.0)/10.0 );    // ตัวอย่างส่งแบบ key & value จาก MCU ปัจจุบันไปอีก MCU
  });

}

void loop() {
  BeeNeXT.update();
}

// ตัวอย่าง รับค่า value ที่ส่งมาจาก MCU อื่น ด้วย key "LED" 
// แล้วนำค่า value มากำหนดให้ led ที่ต่อกับ MCU นี้

BEENEXT_DATA(){
  if(BeeNeXT.key() == "LED") {                      // เช็คว่า key ที่ส่งมาตรงกับ "LED" หรือไม่
    bool value = (bool) BeeNeXT.value().toInt();    // เปลี่ยนให้ value ที่ส่งมา ไปเป็นข้อมูลแบบ boolean  ; อาจจะเปลี่ยนไปค่าแบบต่างๆได้เช่น BeeNeXT.toString(), BeeNeXT.toInt(), BeeNeXT.toFloat(), BeeNeXT.toBool()  ได้ตามต้องการ
    digitalWrite(LED_PIN, value); 
  }
}



