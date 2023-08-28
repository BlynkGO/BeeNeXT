#include <BlynkGOv3.h>

typedef struct _mydata{
  float     a;
  uint16_t  b;
  float     c;
} MyData;

MyData data;

void setup() {
  Serial.begin(9600);
  BlynkGO.begin();
  BeeNeXT.enable(true);
  //GRAPHIC::design(); // ออกแบบกราฟิก
}

void loop() {
  BlynkGO.update();
}

BEENEXT_DATA(){
  if(BeeNeXT.key() == "DATA"){
    size_t sz = BeeNeXT.value((uint8_t*)&data,sizeof(data));
    if(sz == sizeof(data)){
      Serial.println(data.a);
      Serial.println(data.b);
      Serial.println(data.c);
    }
  }
}



