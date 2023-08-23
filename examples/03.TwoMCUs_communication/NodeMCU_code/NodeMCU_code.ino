#include <ESP8266WiFi.h>
#include <BeeNeXT.h>

#define SSID        "------------------"
#define PASSWORD    "------------------"
#define LED_PIN     2

#define LED_ON      LOW
#define LED_OFF     HIGH

void setup() {
  Serial.begin(115200); Serial.println();

  // --- แบบใช้ Software Serial ----------
  BeeNeXT.begin(D1, D2 );  // BeeNeXT เริ่มทำงานด้วย Soft Serial ขา RX D1, TX D2
  // ----------------------------------------

//  Serial.println("[WiFi] connecting....");
//  WiFi.begin(SSID, PASSWORD);
//  while(!WiFi.isConnected()) delay(400);
//  Serial.print("[WiFi] connected. IP : ");
//  Serial.println(WiFi.localIP());


  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    // คำสั่ง BeeNeXT.send( key, value); สำหรับ ส่งค่า จาก MCU ปัจจุบัน ไปยังอีก MCU
    float temp  =  random(100.0)/10.0;
    float humid =  random(100.0)/10.0;
    Serial.println(temp);
    Serial.println(humid);
    BeeNeXT.send("TEMP" , temp );    // ส่งไปยังอีก MCU ด้วย key "TEMP"  และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
    BeeNeXT.send("HUMID", humid );    // ส่งไปยังอีก MCU ด้วย key "HUMID" และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
  });

}

void loop() {
  BeeNeXT.update();
}

