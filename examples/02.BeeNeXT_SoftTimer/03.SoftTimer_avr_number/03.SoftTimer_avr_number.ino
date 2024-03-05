#include <BeeNeXT.h>

int sum; int analog_min; int analog_max;

void setup() {
  Serial.begin(115200); Serial.println();

  static SoftTimer timer;
  timer.setInterval(5000, [](){                   // ตั้งเวลาทำงานทุกๆ 5 วินาที
    Serial.println("[Start] read data 20 rounds");
    sum = 0; analog_min =0xFFFF; analog_max = 0;       // reset ค่า
    static SoftTimer t;
    t.setInterval(10,20,[](){                     // ตั้งเวลาอ่านทุกๆ 10ms ไป 20 รอบ
      int analog_val = random(100);               // อ่านจริง ใช้ analogRead(A0) เป็นต้น
      analog_min = min(analog_val,analog_min);    // เก็บค่าต่ำสุด
      analog_max = max(analog_val,analog_max);    // เก็บค่าสูงสุด
      sum += analog_val;
      Serial.print("analog_val = "); Serial.println(analog_val);  
    });
    t.ready_cb([](){                              // เมื่อครบ 20 รอบแล้ว
      int analog_avr = (sum - analog_min - analog_max)/18;    // ค่ารวมที่หักค่าต่ำสุด สูงสุดออกไป แล้วทำการเฉลี่ย
      Serial.println("---------------");
      Serial.print  ("analog_avr = "); Serial.println( analog_avr );  // แสดงค่าเฉลี่ย
      Serial.println("---------------");
    });
  },true);
}

void loop() {
  BeeNeXT.update();
}

