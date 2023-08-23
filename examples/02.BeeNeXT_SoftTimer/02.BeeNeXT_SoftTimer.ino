#include <BeeNeXT.h>

#define LED_PIN    13

void setup() {
  Serial.begin(115200); Serial.println();

  pinMode(LED_PIN,OUTPUT);

  static SoftTimer timer;
  timer.setInterval(500,[](){             // ตั้งเวลาทำงานเป็นช่วงๆ แบบนี้จะไม่ถูก blocking code
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
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


