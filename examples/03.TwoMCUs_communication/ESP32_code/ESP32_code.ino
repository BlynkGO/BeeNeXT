#include <WiFi.h>
#include <BeeNeXT.h>

#define SSID        "------------------"
#define PASSWORD    "------------------"
#define LED_PIN     5

void setup() {
  Serial.begin(115200); Serial.println();

  // --- แบบใช้ Serial2 ด้วยขา 16,17 ----------
  Serial2.begin(9600, SERIAL_8N1, 16,17);
  BeeNeXT.begin(&Serial2);  // BeeNeXT เริ่มทำงานด้วย Serial2
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





