#include <BeeNeXT.h>

void setup(){
  Serial.begin(9600); Serial.println();

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    static int c;
    Serial.println(String("จำนวน ") + c++);
  });
}

void loop(){
  BeeNeXT.update();
}

