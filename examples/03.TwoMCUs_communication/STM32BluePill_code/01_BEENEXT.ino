// จุดรับข้อมูลที่ส่งเข้ามาจาก MCU อื่น
BEENEXT_DATA(){
  if(BeeNeXT.key() == "LED") {        // เช็คว่า key ที่ส่งมาจาก MCU อื่น key ตรงกับ "LED" หรือไม่
    bool value = BeeNeXT.toBool();    // เปลี่ยนให้ value ที่ส่งมา ไปเป็นข้อมูลแบบ boolean  ; อาจจะเปลี่ยนไปค่าแบบต่างๆได้เช่น BeeNeXT.toString(), BeeNeXT.toInt(), BeeNeXT.toFloat(), BeeNeXT.toBool()  ได้ตามต้องการ
    digitalWrite(LED_PIN, value);     // ทำค่า value ที่ได้รับ ไปใช้งานได้ตามต้องการ
  }else
  if(BeeNeXT.key() == "MY_NUM") { 
    int num = BeeNeXT.toInt();
    Serial.print("[BeeNeXT] recv : ");
    Serial.println(num);
  }
}

// มีหรือไม่ก็ได้
// BEENEXT_CONNECTED() {
//   Serial.println("[BeeNeXT] connected");
// }

// มีหรือไม่ก็ได้
// BEENEXT_DISCONNECTED() {
//   Serial.println("[BeeNeXT] disconnected");
// }
