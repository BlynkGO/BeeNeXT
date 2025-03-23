#include <BeeNeXT.h>

#define LED_PIN    13

void setup() {
  Serial.begin(115200); Serial.println();

  pinMode(LED_PIN,OUTPUT);

  static SoftTimer timer;
  timer.setInterval(500,10,[](){             // ตั้งเวลาทำงานเป็นช่วงๆ แบบนี้จะไม่ถูก blocking code เป็นจำนวน 10 ครั้ง
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  });
  timer.onFinised([](){
    digitalWrite(LED_PIN,LOW);
    Serial.println("[Timer] done!");
  );

}

void loop() {
  BeeNeXT.update();
}


