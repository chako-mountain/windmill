












//尾翼側で構造体を送信するコード
// #include <Arduino.h>
// #include "SoftwareSerial.h"

// struct dakakuData {
//   float rudder = 0;
//   float elevetor = 0;
// };

// dakakuData data;
// SoftwareSerial mySerial(12, 11);

// void setup() {
//   Serial.begin(115200);
//   mySerial.begin(9600);
// }

// void loop() {
//   data.rudder = random(0, 100);  // ランダムな値
//   data.elevetor = random(0, 100);

//   Serial.print(data.rudder);  // rudderの値を表示

//   mySerial.write((uint8_t*)&data, sizeof(data));
//   Serial.println("Data sent!");
//   delay(100); // 適切な送信間隔を設定
// }







//mainOBC側で構造体を受信するコード
#include <Arduino.h>
#include "SoftwareSerial.h"

struct dakakuData {
  float rudder = 0;
  float elevetor = 0;
};

struct withHight {
  float rudder = 0;
  float elevetor = 0;
  float hight = 0;
};

dakakuData data;
withHight data2;


SoftwareSerial mySerial(12, 11);

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available() >= sizeof(data)) {
    mySerial.readBytes((uint8_t*)&data, sizeof(data));
    Serial.print("rudder: ");
    Serial.println(data.rudder);
    Serial.print("elevetor: ");
    Serial.println(data.elevetor);
  }
  data2.rudder = data.rudder;
  data2.elevetor = data.elevetor;



}















