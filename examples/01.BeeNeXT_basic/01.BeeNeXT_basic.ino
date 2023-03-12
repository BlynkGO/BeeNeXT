#include <BeeNeXT.h>

SoftTimer timer;

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("BeeNeXT Test");
//  BeeNeXT.begin();                                      // แบบนี้ใช้ HardwareSerial Serial
  BeeNeXT.begin(/*RX : Yellow*/ 6 , /* TX : Blue*/ 7 );   // แบบนี้ใช้ SoftwareSerial กำหนด RX, TX

  pinMode(13,OUTPUT);
  timer.setInterval(1000,[](){
    static uint32_t c;
    BeeNeXT.send("MY_NUM", c++);    // ส่งแบบ key & value
    // BeeNeXT.println(c++);        // ส่งแบบพื้นฐาน
    // Serial.println(c++);
  });
}

void loop() {
  BeeNeXT.update();
}

BEENEXT_DATA(){
  Serial.println(String("[BeeNeXT] ")+ BeeNeXT.data());

  if(BeeNeXT.key() == "LED") {   // เมื่อ key ตรงกับ "LED"
    digitalWrite(13, BeeNeXT.toBool() );   // ให้ value เปลี่ยนเป็นข้อมูลแบบ boolean  
  }
}

// SERIAL_DATA(){
//   Serial.println(String("[Serial] ")+ BeeNeXT.data());
// }
