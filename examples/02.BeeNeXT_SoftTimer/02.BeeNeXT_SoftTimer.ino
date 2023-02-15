#include <BeeNeXT.h>

#define LED     13

void setup() {
  Serial.begin(115200); Serial.println();
  BeeNeXT.begin();                                           // แบบนี้ใช้ HardwareSerial Serial
  // BeeNeXT.begin(/*RX : Yellow*/ 6 , /* TX : Blue*/ 7 );   // แบบนี้ใช้ SoftwareSerial กำหนด RX, TX

  pinMode(LED,OUTPUT);

  static SoftTimer timer;
  timer.setInterval(500,[](){             // ตั้งเวลาทำงานเป็นช่วงๆ แบบนี้จะไม่ถูก blocking code
    digitalWrite(LED, !digitalRead(LED));
  });

  Serial.print("Hello");
  static SoftTimer timer_delay;
  timer_delay.delay(5000,[](){            // ตั้งเวลา delay อีก 5 วินาทีค่อยทำงาน แบบนี้จะไม่ถูก blocking code
    Serial.println("World");
  });

  Serial.print(" : <wait 5secs> : ");
}

void loop() {
  BeeNeXT.update();
}


