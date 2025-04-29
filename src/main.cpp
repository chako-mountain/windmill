












// // 尾翼側で構造体を送信するコード
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







// //mainOBC側で構造体を受信するコード
// //また、ESP32からのリクエストに応じて構造体データを送信
// #include <Arduino.h>
// #include "SoftwareSerial.h"
// #include <Wire.h>

// #define SLAVE_ADDRESS 0x08 

// struct dakakuData {
//   float rudder = 0;
//   float elevetor = 0;
// };

// struct withHight {
//   float rudder = 0;
//   float elevetor = 0;
//   float hight = 0;
// };

// dakakuData data;
// withHight data2;

// const int pin = A0;


// SoftwareSerial mySerial(12, 11);

// void sendData() {
//   Wire.write((uint8_t*)&data2, sizeof(data2));
//   Serial.println("data sent to master"); // デバッグ用のメッセージ
// }

// void setup() {
//   Serial.begin(115200);
//   mySerial.begin(9600);
//   Wire.begin(SLAVE_ADDRESS); 
//   Wire.onRequest(sendData);

//   pinMode(pin, INPUT);
// }

// void loop() {
//   if (mySerial.available() >= sizeof(data)) {
//     mySerial.readBytes((uint8_t*)&data, sizeof(data));
    
//   }
//   data2.rudder = data.rudder;
//   data2.elevetor = data.elevetor;

//   int h = 0;
//   int datasum = 0;
//   int rawdata[5];

//   for (int i = 0; i < 4; i++) {
//     rawdata[i] = 0;
//   }


//   for (int i = 0; i < 4; i++) {
//     rawdata[i] = analogRead(pin);
    
//     datasum = datasum + rawdata[i];
//   }


//   int rawmax = 0;
//   int rawmin = 0;

//   for (int i = 0; i < 4; i++) {
//     if (rawmax < rawdata[i]) {
//       rawmax = rawdata[i];
//     }
//   }

//   for (int i = 0; i < 4; i++) {
//     if (rawmin > rawdata[i]) {
//       rawmin = rawdata[i];
//     }
//   }

//   h = (datasum - rawmax - rawmin ) / 3;
//   Serial.println(h);
  
//   data2.hight = h;
  
//   Serial.println(millis());
//   Serial.print("rudder: dao");
//   Serial.println(data.rudder);
//   Serial.print("elevetor: ");
//   Serial.println(data.elevetor);
//   Serial.print("hight: ");
//   Serial.println(data2.hight);

//   delay(100); // 適切な受信間隔を設定

// }








//ESP32側で、とりあえずUnoからのデータを受信
// #include <Arduino.h>
// #include <Wire.h>

// struct withHight {
//   float rudder = 0;
//   float elevetor = 0;
//   float hight = 0;
// };

// #define SLAVE_ADDRESS 0x08 // スレーブのアドレス



// withHight maindata; // 受信データ格納用構造体

// void setup() {
//   Serial.begin(115200);
//   Wire.begin(); // マスターとして初期化
// }

// void loop() {
//   Wire.requestFrom(SLAVE_ADDRESS, sizeof(maindata)); // 構造体サイズ分データ要求

//   if (Wire.available() >= sizeof(maindata)) { // 必要なバイト数があるか確認
//     Wire.readBytes((uint8_t*)&maindata, sizeof(maindata)); // 構造体全体を取得
    
//     // 受信データを表示
//     Serial.print("Received rudder: ");
//     Serial.println(maindata.rudder);
//     Serial.print("Received elevetor: ");
//     Serial.println(maindata.elevetor);
//     Serial.print("Received hight: ");
//     Serial.println(maindata.hight);
//   }
  
//   delay(100); // 送信頻度調整
// }