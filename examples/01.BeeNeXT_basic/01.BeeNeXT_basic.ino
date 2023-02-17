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
    BeeNeXT.println(c++);
    // Serial.println(c++);
  });
}

void loop() {
  BeeNeXT.update();
}

BEENEXT_DATA(){
  Serial.println(String("[BeeNeXT] ")+ BeeNeXT.toString());
  digitalWrite(13, BeeNeXT.toBool() );
}

// SERIAL_DATA(){
//   Serial.println(String("[Serial] ")+ BeeNeXT.toString());
// }
