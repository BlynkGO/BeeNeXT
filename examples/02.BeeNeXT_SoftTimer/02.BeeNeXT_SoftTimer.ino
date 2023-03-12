#include <BeeNeXT.h>

#define LED_PIN    13

void setup() {
  Serial.begin(115200); Serial.println();

  // --- แบบใช้ Hardware Serial ----------
  BeeNeXT.begin();                         // BeeNeXT เริ่มทำงานด้วย Serial

  // --- แบบใช้ Hardware Serial2 ---------
  // Serial2.begin(9600, SERIAL_8N1, 16,17);  // Serial2 ของ ESP32
  // Serial2.begin(9600);                     // Serial2 ของ Arduino MEGA ขา RX16, TX17
  // BeeNeXT.begin(&Serial2);                 // BeeNeXT เริ่มทำงานด้วย Serial2

  // --- แบบใช้ Software Serial ----------
  // BeeNeXT.begin(/*RX : Yellow*/ 6 , /* TX : Blue*/ 7 );  // BeeNeXT เริ่มทำงานด้วย Soft Serial ขา RX6, TX7


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


