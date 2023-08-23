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
    BeeNeXT.send("TEMP" , (float) random(100.0)/10.0 );    // ตัวอย่างส่งแบบ key & value จาก MCU ปัจจุบันไปอีก MCU
    BeeNeXT.send("HUMID", (float) random(100.0)/10.0 );    // ตัวอย่างส่งแบบ key & value จาก MCU ปัจจุบันไปอีก MCU
  });

}

void loop() {
  BeeNeXT.update();
}



