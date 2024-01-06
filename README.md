# BeeNeXT

**ไลบรารี่ ลิขสิทธิ์ สำหรับ ผู้ใช้ จอสัมผัส BeeNeXT เท่านั้น**  
ให้ MCU ต่างๆ เช่น Arduino UNO, Arduino MEGA, Arduino Nano, ESP8266, ESP32, STM32 ฯลฯ  
เชื่อมต่อกับ จอสัมผัส BeeNeXT

Version 3.0.5 : @06/01/67  
- Support ESP32S2, ESP32C3 on Serial1  
   - ESP32S2        : on Serial1 18,17
   - ESP32C3        : on Serial1 18,19
   - ESP32, ESP32S3 : on Serial2 16,17
    
Version 3.0.4 : @19/10/66  
- เพิ่ม MCU_DATA, LCD_DATA ซึ่ง alias ไปยัง BEENEXT_DATA
- เพิ่ม MCU, LCD  ซึ่ง alias ไปยัง BeeNeXT
  
Versoin 3.0.3 : @ 30/09/66
- ยกเลิก mem.h,mem.c กลับไปใน free(..) แทน
- fix ปัญหาสำคัญใช้ memset(..) ผิด แก้ให้ถูกแล้ว (สลับ 2 parameter สุดท้าย)
  
Versoin 3.0.2 : @ 30/09/66
- เพิ่ม mem.h,mem.c ให้ ไม่ต้องติด warning
  
Version3.0.1
- ปรับขนาด buffer ได้ ใน beenext_config.h

Version3.0.0
- โปรโตคอลใหม่ มี checksum CRC16 และรองรับ จัดส่งแบบ byte ตามจำนวนได้
- รองรับ STM32

