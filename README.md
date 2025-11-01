# BeeNeXT

**ไลบรารี่ ลิขสิทธิ์ สำหรับ ผู้ใช้ จอสัมผัส BeeNeXT เท่านั้น**  
ให้ MCU ต่างๆ เช่น Arduino UNO, Arduino MEGA, Arduino Nano, ESP8266, ESP32, STM32 ฯลฯ  
เชื่อมต่อกับ จอสัมผัส BeeNeXT  

Version 3.1.15  @01/11/68  
- fix SoftTimer delay(...) ที่บน PC รัน nested lambda ไม่ได้
  
Version 3.1.14 @24/09/68  
- รองรับ USB CDC on boot : Enable ได้ด้วย  
    
Version 3.1.13  @22/09/68  
- fix BeeUart ให้มีคำสัง print, และ กรอง [BN] ออก  
  
Version 3.1.12  @27/06/68  
- fix for BeeNeXT1.9IPS, BeeNeXT3.5, BeeNeXT3.5C  
  
Version 3.1.11  @04/04/68  
- fix BeeMQTT ฝั่ง จอ BeeNeXT ไม่ให้ มี error BeeMQTT ที่จะไม่มีให้ใช้  
  
Version 3.1.10 : @04/04/68   
- fix BEENEXT_USE_BEEUART สำหรับ ESP32P4  
- fix BeeMQTT ฝั่ง MCU หลัก  
  
Version 3.1.9 : @29/03/68   
- fix BEENEXT_USE_BEEUART เพิ่ม onData ฝั่งจอ BeeNeXT  
- ปรับแก้ตัวอย่าง  
  
Version 3.1.8 : @29/03/68   
- เอาขา 20 เป็น OUTPUT LOW ออก เนื่องจากไปชน ขา I2C ของ Touch สำหรับ BeeNeXT4.3,4.3C,4.3IPS  
- เพิ่ม BeeUART.h/.cpp  BEENEXT_USE_BEEUART  
  
Version 3.1.7 : @28/03/68   
- BeeI2C สำหรับ BeeNeXT4.3, 4.3C, 4.3IPS เพิ่มขา 20 ทำเป็น GND (OUTPUT/LOW)   
  
Version 3.1.6 : @23/03/68  
- รองรับ BEENEXT_USE_BEEI2C (default) แต่แรก  
- รองรับ BEENEXT_USE_BEEMQTT เชื่อมต่อแบบไร้สาย 
- รองรับ BEENEXT_USE_BEEUART_CRC16   
- SoftTimer มี onFinished แทน ready_cb  
  
Version 3.1.5 : @21/03/68  
- รองรับ ESP32P4, ESP32C6 ด้วย  

Version 3.1.4 : @17/03/68  
- ESP8266 / ESP32 เพิ่ม ::delay(1) ขณะ run ป้องกัน WDT ให้สามารถกระจายไปยัง task อื่นทำงานได้  
  
Version 3.1.3 : @21/12/67  
- ESP32 รองรับ CDC  ( HWCDC, USBCDC )      
  
Version 3.1.2 : @26/06/67  
- เปลี่ยนชื่อคำสั่ง replay() เป็น resume() ที่ตรงความหมาย แทน      
  
Version 3.1.1 : @05/03/67  
- เพิ่ม free(memory) ให้คืน mem หลัง del()    
  
Version 3.1.0 : @04/03/67  
- SoftTimer เพิ่มให้ setInterval สามารถตั้งจำนวนรอบทำงานได้
  เมื่อสิ้นสุดให้ไปทำงาน ready_cb ที่กำหนด  
  
Version 3.0.6 : @04/03/67  
- fix warning compiling บน AVR  
  
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

