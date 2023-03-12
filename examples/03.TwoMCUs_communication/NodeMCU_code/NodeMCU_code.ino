#include <ESP8266WiFi.h>
#include <BeeNeXT.h>

#define LED_PIN    2

void setup() {
  Serial.begin(115200); Serial.println();

  // --- แบบใช้ Software Serial ----------
  BeeNeXT.begin(D1, D2 );  // BeeNeXT เริ่มทำงานด้วย Soft Serial ขา RX D1, TX D2

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

// ตัวอย่าง รับค่า value ที่ส่งมาจาก MCU อื่น ด้วย key "LED" 
// แล้วนำค่า value มากำหนดให้ led ที่ต่อกับ MCU นี้

BEENEXT_DATA(){
  String key = BeeNeXT.key();
  if( key == "LED") {                               // เช็คว่า key ที่ส่งมาตรงกับ "LED" หรือไม่
    bool value = (bool) BeeNeXT.value().toInt();    // เปลี่ยนให้ value ที่ส่งมา ไปเป็นข้อมูลแบบ boolean  ; อาจจะเปลี่ยนไปค่าแบบต่างๆได้เช่น BeeNeXT.toString(), BeeNeXT.toInt(), BeeNeXT.toFloat(), BeeNeXT.toBool()  ได้ตามต้องการ
    digitalWrite(LED_PIN, value); 
  }else
  if( key == "TEMP" ) {
    float temp = BeeNeXT.toFloat();
    Serial.printf("[TEMP] %.2f\n", temp);
  }else
  if( key == "HUMID" ){
    float humid = BeeNeXT.toFloat();
    Serial.printf("[HUMID] %.2f\n", humid);    
  }
}



