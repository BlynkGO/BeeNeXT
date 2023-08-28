#include <BeeNeXT.h>

typedef struct _mydata{
  float     a;
  uint16_t  b;
  float     c;
} MyData;

MyData data;

void setup() {
  Serial.begin(9600); Serial.println();
  Serial2.begin(9600, SERIAL_8N1, 16,17);  // ESP32 ต่อขา 16,17
  BeeNeXT.begin(&Serial2);
  //ตั้งเวลาจัดส่ง ทุกๆ 500ms แบบทั้ง struct
  static SoftTimer timer;
  timer.setInterval(500,[](){
    data.a = 23.43; data.b = 7; data.c= 34.23;
    BeeNeXT.send("DATA", (uint8_t*)&data, sizeof(data));
  });
}

void loop() {
  BeeNeXT.update();
}

