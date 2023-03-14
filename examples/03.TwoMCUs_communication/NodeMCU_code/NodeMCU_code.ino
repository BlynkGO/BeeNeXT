#include <ESP8266WiFi.h>
#include <BeeNeXT.h>

#define SSID        "------------------"
#define PASSWORD    "------------------"
#define LED_PIN     2


void setup() {
  Serial.begin(115200); Serial.println();

  // --- แบบใช้ Software Serial ----------
  BeeNeXT.begin(D1, D2 );  // BeeNeXT เริ่มทำงานด้วย Soft Serial ขา RX D1, TX D2
  // ----------------------------------------

  Serial.println("[WiFi] connecting....");
  WiFi.begin(SSID, PASSWORD);
  while(!WiFi.isConnected()) delay(400);
  Serial.print("[WiFi] connected. IP : ");
  Serial.println(WiFi.localIP());


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


