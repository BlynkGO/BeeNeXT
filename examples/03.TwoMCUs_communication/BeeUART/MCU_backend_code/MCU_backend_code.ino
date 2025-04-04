// beenext_config.h กำหนดดังนี้
//       #define BEENEXT_USE_BEEI2C              1
//       #define BEENEXT_USE_BEEUART             0
//       #define BEENEXT_USE_BEEMQTT             0

#include <BeeNeXT.h>

#if defined(ESP8266) || defined(ESP32)
  #define LED_PIN     2
  #define LED_ON      LOW
  #define LED_OFF     HIGH
#else  // UNO, MEGA, NANO
  #define LED_PIN     13
  #define LED_ON      HIGH
  #define LED_OFF     LOW
#endif

void setup() {
  Serial.begin(115200); Serial.println();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  /*************************************
   * ใช้ Serial เป็น ขาต่อ ไปยัง UART ฝั่งจอ
   *********************/
  LCD.begin([](String key, String value){
    Serial.println(key + " ---> " + value);
    if(key.startsWith("LED")) {
      bool led_state = value.toInt();
      digitalWrite(LED_PIN, led_state? LED_ON : LED_OFF);
    }
  });
  /*************************************
   * ใช้ Serial2 เป็น ขาต่อ ไปยัง UART ฝั่งจอ  (สำหรับ MEGA, ESP32)
   *********************/
  // Serial2.begin(9600);  // MEGA ขา 17 (rx2), 16 (tx2);  ESP32 ขา 17 (rx2), 16 (tx2)
  // Serial2.begin(9600, SERIAL_8N1, 21, 22);  // ESP32 กำหนดขาเองได้
  // Serial2.begin(9600, SERIAL_8N1, 17, 18);  // ESP32S3 กำหนดขาเองได้
  // LCD.begin(&Serial2, [](String key, String value){
  //   Serial.println(key + " ---> " + value);
  //   if(key.startsWith("LED")) {
  //     bool led_state = value.toInt();
  //     digitalWrite(LED_PIN, led_state? LED_ON : LED_OFF);
  //   }
  // });
  /*************************************
   * ใช้ SoftwareSerial เป็น ขาต่อ ไปยัง UART ฝั่งจอ เช่น UNO, NANO, MEGA, ESP8266, ESP32
   *********************/
  // LCD.begin(16,17, 9600, [](String key, String value)  // หรือ
  // LCD.begin(16,17, [](String key, String value)
  // {
  //   Serial.println(key + " ---> " + value);
  //   if(key.startsWith("LED")) {
  //     bool led_state = value.toInt();
  //     digitalWrite(LED_PIN, led_state? LED_ON : LED_OFF);
  //   }
  // });

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    float temp  = (float) random(200, 500)/10.0;  // ของจริงให้อ่านค่าจาก sensor
    float humid = (float) random(600, 900)/10.0;  // ของจริงให้อ่านค่าจาก sensor
    Serial.printf("[TEMP] %.1f ; [HUMID] %.1f\n", temp, humid);
    LCD.print("TEMP" , temp );    // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "TEMP"  และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
    LCD.print("HUMID", humid );    // ส่งไปแสดงยังจอ BeeNeXT ด้วย key "HUMID" และ value ที่ส่ง ส่งได้ทั้ง จำนวนเต็ม, จำนวนจริง หรือ ข้อความก็ได้ ตามต้องการ
  });  
}

void loop() {
  BeeNeXT.update();
}
