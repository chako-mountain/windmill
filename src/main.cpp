// [env:esp32]
// platform = espressif32
// board = esp32dev
// framework = arduino
// lib_deps = 
//     ; regenbogencode/ESPNowW@^1.0.2
//     ; mikalhart/TinyGPSPlus@^1.1.0
//     ; adafruit/Adafruit BNO055@^1.6.4
//     mobizt/Firebase Arduino Client Library for ESP8266 and ESP32@^4.4.17
// monitor_speed = 115200







// // 尾翼側で構造体を送信するコード
// #include <Arduino.h>
// #include "SoftwareSerial.h"

// struct dakakuData {
//   float rudder = 0;
//   float elevetor = 0;
// };

// //12番ピンをRX、11番ピンをTXに設定
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













// 実際につかった！！
//操縦桿から舵角を取得して、メイン処理をするマイコンに送信している

// // 操縦桿側でセンサーのアナログ値を読み取り、キャリブレーションして構造体を送信するコード

// #include <Arduino.h>
// #include "SoftwareSerial.h"

// // 構造体の定義
// struct SensorData {
//   int rawValue;      // アナログ値
//   float angle;       // 角度
//   int offset;        // キャリブレーション用オフセット
// };

// struct senddata {
//   float rudder;
//   float elevetor;
// };


// // ピンの定義
// const int analogPin1 = A0;
// const int analogPin2 = A1;

// // 構造体のインスタンスを作成
// SensorData sensor1;
// SensorData sensor2;
// senddata data;
// SoftwareSerial mySerial(11, 10);

// void setup() {
//   Serial.begin(115200);
//   mySerial.begin(9600);
//   pinMode(analogPin1, INPUT);
//   pinMode(analogPin2, INPUT);

//   // キャリブレーション（起動時の値をオフセットとして保存）
//   sensor1.offset = analogRead(analogPin1);
//   sensor2.offset = analogRead(analogPin2);

//   Serial.print("Calibration Offset Sensor 1: ");
//   Serial.println(sensor1.offset);
//   Serial.print("Calibration Offset Sensor 2: ");
//   Serial.println(sensor2.offset);
// }

// void loop() {
//   // センサー1のデータを取得
//   sensor1.rawValue = analogRead(analogPin1) - sensor1.offset;
//   sensor1.angle = map(sensor1.rawValue, 0, 1023, 0, 360);

//   // センサー2のデータを取得
//   sensor2.rawValue = analogRead(analogPin2) - sensor2.offset;
//   sensor2.angle = map(sensor2.rawValue, 0, 1023, 0, 360);

//   // 結果をシリアルモニタに表示
//   Serial.print("Sensor 1 - Calibrated Raw Value: ");
//   Serial.print(sensor1.rawValue);
//   Serial.print(", Angle: ");
//   Serial.print(sensor1.angle);
//   Serial.println(" degrees");

//   // Serial.print("Sensor 2 - Calibrated Raw Value: ");
//   // Serial.print(sensor2.rawValue);
//   // Serial.print(", Angle: ");
//   // Serial.print(sensor2.angle);
//   // Serial.println(" degrees");

//   data.elevetor = sensor1.angle;
//   data.rudder = sensor2.angle;

//   mySerial.write((uint8_t*)&data, sizeof(data));



//   delay(100);
// }





















// //mainOBC側で構造体を受信するコード
// //また、ESP32からのリクエストに応じて構造体データを送信
// #include <Arduino.h>
// #include "SoftwareSerial.h"
// #include <Wire.h>

// const int DATA = 8;
// const int LATCH = 10;
// const int CLOCK = 9;

// //舵角系表示
// const int PIN_SER   = 7;
// const int PIN_LATCH = 5;
// const int PIN_CLK   = 6;

// byte PATTERN[] = {
//   0b00000011, // 0
//   0b10011111, // 1
//   0b00100101, // 2
//   0b00001101, // 3
//   0b10011001, // 4
//   0b01001001, // 5
//   0b01000001, // 6
//   0b00011111, // 7
//   0b00000001, // 8
//   0b00001001  // 9
// };



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

// // 12番ピンをRX、11番ピンをTXに設定
// //舵角情報はこれでいい。
// SoftwareSerial mySerial(11, 12);

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

//   pinMode(DATA, OUTPUT);
//   pinMode(LATCH, OUTPUT);
//   pinMode(CLOCK, OUTPUT);
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
//   int rawdatar; 

//   for (int i = 0; i < 4; i++) {
//     rawdata[i] = 0;
//   }


//   for (int i = 0; i < 4; i++) {
//       rawdatar = analogRead(pin);

//       double voltage =rawdatar/1023.0*5.0;

//       rawdata[i]=voltage/4.9*1000*2;
    
//       datasum = datasum + rawdata[i];
//     }


//     int rawmax = 0;
//     int rawmin = 0;

//     for (int i = 0; i < 4; i++) {
//       if (rawmax < rawdata[i]) {
//         rawmax = rawdata[i];
//       }
//     }

//     for (int i = 0; i < 4; i++) {
//       if (rawmin > rawdata[i]) {
//         rawmin = rawdata[i];
//       }
//     }

//     h = (datasum - rawmax - rawmin ) / 3;
//     Serial.println(h);

//     int hundreds = h / 100;         // 100の位を取得
//     int tens = (h / 10) % 10;       // 10の位を取得
//     int units = h % 10;             // 1の位を取得

//     digitalWrite(LATCH, LOW);
//     shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[hundreds]);
//     shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//     digitalWrite(LATCH, HIGH);
  
//     data2.hight = h;
  
//     Serial.println(millis());
//     Serial.print("rudder: dao");
//     Serial.println(data2.rudder);
//     Serial.print("elevetor: ");
//     Serial.println(data2.elevetor);
//     Serial.print("hight: ");
//     Serial.println(data2.hight);


//     elevetorToPattern(data2.elevetor);

//     delay(100); // 適切な受信間隔を設定

// }





// // //mainOBC側で構造体を受信するコード
// // //また、ESP32からのリクエストに応じて構造体データを送信
// // #include <Arduino.h>
// // #include "SoftwareSerial.h"
// // #include <Wire.h>

// const int DATA = 8;
// const int LATCH = 10;
// const int CLOCK = 9;

// //舵角系表示
// const int PIN_SER   = 7;
// const int PIN_LATCH = 5;
// const int PIN_CLK   = 6;

// byte PATTERN[] = {
//   0b00000011, // 0
//   0b10011111, // 1
//   0b00100101, // 2
//   0b00001101, // 3
//   0b10011001, // 4
//   0b01001001, // 5
//   0b01000001, // 6
//   0b00011111, // 7
//   0b00000001, // 8
//   0b00001001  // 9
// };



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

// // 12番ピンをRX、11番ピンをTXに設定
// //舵角情報はこれでいい。
// SoftwareSerial mySerial(11, 12);

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

//   pinMode(DATA, OUTPUT);
//   pinMode(LATCH, OUTPUT);
//   pinMode(CLOCK, OUTPUT);
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
//   int rawdatar; 

//   for (int i = 0; i < 4; i++) {
//     rawdata[i] = 0;
//   }


//   for (int i = 0; i < 4; i++) {
//     rawdatar = analogRead(pin);

//     double voltage =rawdatar/1023.0*5.0;

//     rawdata[i]=voltage/4.9*1000*2;
    
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

//   int hundreds = h / 100;         // 100の位を取得
//   int tens = (h / 10) % 10;       // 10の位を取得
//   int units = h % 10;             // 1の位を取得

//   digitalWrite(LATCH, LOW);
//   shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[hundreds]);
//   shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//   digitalWrite(LATCH, HIGH);
  
//   data2.hight = h;
  
//   Serial.println(millis());
//   Serial.print("rudder: dao");
//   Serial.println(data2.rudder);
//   Serial.print("elevetor: ");
//   Serial.println(data2.elevetor);
//   Serial.print("hight: ");
//   Serial.println(data2.hight);


//   elevetorToPattern(data2.elevetor);

//   delay(100); // 適切な受信間隔を設定

// }

















// //mainOBC側で構造体を受信するコード
// //また、ESP32からのリクエストに応じて構造体データを送信


// #include <Arduino.h>
// #include "SoftwareSerial.h"
// #include <Wire.h>

// const int DATA = 8;
// const int LATCH = 10;
// const int CLOCK = 9;

// // //舵角系表示
// // const int PIN_SER   = 7;
// // const int PIN_LATCH = 5;
// // const int PIN_CLK   = 6;

// byte PATTERN[] = {
//   0b00000011, // 0
//   0b10011111, // 1
//   0b00100101, // 2
//   0b00001101, // 3
//   0b10011001, // 4
//   0b01001001, // 5
//   0b01000001, // 6
//   0b00011111, // 7
//   0b00000001, // 8
//   0b00001001  // 9
// };

// byte patterns[] = {
//   0b11000000,
//   0b01100000, // -40
//   0b00110000,
//   0b00011000,
//   0b00001100,
//   0b00000110,
//   0b00000011,
//   0b00000000, // 0
//   // 0b00011111,
// };



// void elevetorToPattern(float elevetor) {
//   // 範囲外は端に丸める

//   // digitalWrite(LATCH, LOW);
//   // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[hundreds]);
//   // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//   // digitalWrite(LATCH, HIGH);
//   int number = 0;
//   if (elevetor < -40) {
//       number= 0;
//       digitalWrite(PIN_LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);
//       digitalWrite(PIN_LATCH, HIGH);
//   } else if (elevetor > -40 && elevetor < -26) {
//       number=1;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > -25 && elevetor < -13) {
//       number=2;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > -12 && elevetor < -0) {
//       number=3;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > 0 && elevetor < 13) {
//       number=4;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > 13 && elevetor < 26) {
//     number=5;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > 26 && elevetor < 40) {
//     number=6;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, CLOCK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else {
//     number=0;
//     digitalWrite(PIN_LATCH, LOW);
//     shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//     // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//     digitalWrite(PIN_LATCH, HIGH);
//   }
// }










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

// // 12番ピンをRX、11番ピンをTXに設定
// //舵角情報はこれでいい。
// SoftwareSerial mySerial(11, 12);

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

//   pinMode(DATA, OUTPUT);
//   pinMode(LATCH, OUTPUT);
//   pinMode(CLOCK, OUTPUT);

//   // pinMode(PIN_SER, OUTPUT);
//   // pinMode(PIN_LATCH, OUTPUT);
//   // pinMode(PIN_CLK, OUTPUT);
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
//   int rawdatar; 

//   for (int i = 0; i < 4; i++) {
//     rawdata[i] = 0;
//   }


//   for (int i = 0; i < 4; i++) {
//       rawdatar = analogRead(pin);

//       double voltage =rawdatar/1023.0*5.0;

//       rawdata[i]=voltage/4.9*1000*2;
    
//       datasum = datasum + rawdata[i];
//     }


//     int rawmax = 0;
//     int rawmin = 0;

//     for (int i = 0; i < 4; i++) {
//       if (rawmax < rawdata[i]) {
//         rawmax = rawdata[i];
//       }
//     }

//     for (int i = 0; i < 4; i++) {
//       if (rawmin > rawdata[i]) {
//         rawmin = rawdata[i];
//       }
//     }

//     h = (datasum - rawmax - rawmin ) / 3;
//     Serial.println(h);

//     int hundreds = h / 100;         // 100の位を取得
//     int tens = (h / 10) % 10;       // 10の位を取得
//     int units = h % 10;             // 1の位を取得

//     digitalWrite(LATCH, LOW);
//     shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[hundreds]);
//     shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//     digitalWrite(LATCH, HIGH);
  
//     data2.hight = h;
  
//     Serial.println(millis());
//     Serial.print("rudder: dao");
//     Serial.println(data2.rudder);
//     Serial.print("elevetor: ");
//     Serial.println(data2.elevetor);
//     Serial.print("hight: ");
//     Serial.println(data2.hight);


//     // elevetorToPattern(data2.elevetor);

//     delay(100); // 適切な受信間隔を設定

// }




















// 実際に使った！！
//mainOBC側で構造体を受信するコード
// 送信されてきた舵角を取得。また、高度計から高度を取得し、表示している。
//また、ESP32からのリクエストに応じて構造体データを送信

// #include <Arduino.h>
// #include "SoftwareSerial.h"
// #include <Wire.h>

// const int DATA = 8;
// const int LATCH = 10;
// const int CLOCK = 9;

//舵角系表示
// const int PIN_SER   = 7;
// const int PIN_LATCH = 5;
// const int PIN_CLK   = 6;

// byte PATTERN[] = {
//   0b00000011, // 0
//   0b10011111, // 1
//   0b00100101, // 2
//   0b00001101, // 3
//   0b10011001, // 4
//   0b01001001, // 5
//   0b01000001, // 6
//   0b00011111, // 7
//   0b00000001, // 8
//   0b00001001  // 9
// };

// byte patterns[] = {
//   0b11000000,
//   0b01100000, // -40
//   0b00110000,
//   0b00011000,
//   0b00001100,
//   0b00000110,
//   0b00000011,
//   0b00000000, // 0
//   // 0b00011111,
// };

// byte patterns[] = {
//   0b10000001, // 両端だけ点灯
//   0b11000011, // 両端+隣
//   0b11100111, // 真ん中4つ点灯
//   0b11111100, // 下2つ点灯
//   0b00000011, // 上2つ点灯
//   0b10101010, // 交互点灯
//   0b01010101, // 交互点灯(逆)
//   0b00000000  // 全点灯
// };



// void elevetorToPattern(float elevetor) {
//   // 範囲外は端に丸める

//   // digitalWrite(LATCH, LOW);
//   // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[hundreds]);
//   // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//   // digitalWrite(LATCH, HIGH);
//   int number = 0;
//   if (elevetor < -40) {
//       number= 0;
//       digitalWrite(PIN_LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);
//       digitalWrite(PIN_LATCH, HIGH);
//   } else if (elevetor > -40 && elevetor < -26) {
//       number=1;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > -25 && elevetor < -13) {
//       number=2;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > -12 && elevetor < -0) {
//       number=3;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > 0 && elevetor < 13) {
//       number=4;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > 13 && elevetor < 26) {
//     number=5;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > 26 && elevetor < 40) {
//     number=6;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, CLOCK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else {
//     number=0;
//     digitalWrite(PIN_LATCH, LOW);
//     shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//     // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//     digitalWrite(PIN_LATCH, HIGH);
//   }
// }







// void elevetorToPattern(float elevetor) {
//   int number = 0;
//   if (elevetor < 20) {
//     number = 0;
//   } else if (elevetor < 40) {
//     number = 1;
//   } else if (elevetor < 60) {
//     number = 2;
//   } else if (elevetor < 80) {
//     number = 3;
//   } else if (elevetor < 100) {
//     number = 4;
//   } else if (elevetor < 120) {
//     number = 5;
//   } else {
//     number = 6;
//   }
//   digitalWrite(PIN_LATCH, LOW);
//   shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//   digitalWrite(PIN_LATCH, HIGH);
// }


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

// // 12番ピンをRX、11番ピンをTXに設定
// //舵角情報はこれでいい。
// SoftwareSerial mySerial(11, 12);

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

//   pinMode(DATA, OUTPUT);
//   pinMode(LATCH, OUTPUT);
//   pinMode(CLOCK, OUTPUT);

//   // pinMode(PIN_SER, OUTPUT);
//   // pinMode(PIN_LATCH, OUTPUT);
//   // pinMode(PIN_CLK, OUTPUT);
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
//   int rawdatar; 

//   for (int i = 0; i < 4; i++) {
//     rawdata[i] = 0;
//   }


//   for (int i = 0; i < 4; i++) {
//       rawdatar = analogRead(pin);

//       double voltage =rawdatar/1023.0*5.0;

//       rawdata[i]=voltage/4.9*1000*2;
    
//       datasum = datasum + rawdata[i];
//     }


//     int rawmax = 0;
//     int rawmin = 0;

//     for (int i = 0; i < 4; i++) {
//       if (rawmax < rawdata[i]) {
//         rawmax = rawdata[i];
//       }
//     }

//     for (int i = 0; i < 4; i++) {
//       if (rawmin > rawdata[i]) {
//         rawmin = rawdata[i];
//       }
//     }

//     h = (datasum - rawmax - rawmin ) / 3;
//     Serial.println(h);

//     int hundreds = h / 100;         // 100の位を取得
//     int tens = (h / 10) % 10;       // 10の位を取得
//     int units = h % 10;             // 1の位を取得

//     digitalWrite(LATCH, LOW);
//     shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[hundreds]);
//     shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//     digitalWrite(LATCH, HIGH);
  
//     data2.hight = h;
  
//     Serial.println(millis());
//     Serial.print("rudder: dao");
//     Serial.println(data2.rudder);
//     Serial.print("elevetor: ");
//     Serial.println(data2.elevetor);
//     Serial.print("hight: ");
//     Serial.println(data2.hight);


//     // elevetorToPattern(data2.elevetor);

//     delay(100); // 適切な受信間隔を設定

// }


















// //mainOBC側で構造体を受信するコード
// //また、ESP32からのリクエストに応じて構造体データを送信
// #include <Arduino.h>
// #include "SoftwareSerial.h"
// #include <Wire.h>

// const int DATA = 8;
// const int LATCH = 10;
// const int CLOCK = 9;

// //舵角系表示
// const int PIN_SER   = 7;
// const int PIN_LATCH = 5;
// const int PIN_CLK   = 6;

// byte PATTERN[] = {
//   0b00000011, // 0
//   0b10011111, // 1
//   0b00100101, // 2
//   0b00001101, // 3
//   0b10011001, // 4
//   0b01001001, // 5
//   0b01000001, // 6
//   0b00011111, // 7
//   0b00000001, // 8
//   0b00001001  // 9
// };

// byte patterns[] = {
//   0b11000000,
//   0b01100000, // -40
//   0b00110000,
//   0b00011000,
//   0b00001100,
//   0b00000110,
//   0b00000011,
//   0b00000000, // 0
//   // 0b00011111,
// };



// void elevetorToPattern(float elevetor) {
//   // 範囲外は端に丸める

//   // digitalWrite(LATCH, LOW);
//   // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[hundreds]);
//   // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//   // digitalWrite(LATCH, HIGH);
//   int number = 0;
//   if (elevetor < -40) {
//       number= 0;
//       digitalWrite(PIN_LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);
//       digitalWrite(PIN_LATCH, HIGH);
//   } else if (elevetor > -40 && elevetor < -26) {
//       number=1;
//       digitalWrite(LATCH, LOW);
//       shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[number]);
//       // shiftOut(DATA, CLOCK, LSBFIRST, PATTERN[tens]);  
//       digitalWrite(LATCH, HIGH);
//   } else if (elevetor > -25 && elevetor < -13) {
//       number=2;
//       digitalWrite(LATCH, LOW);
//   }
//   Serial.println("SD Card initialized");

//   // IRremote初期化
//   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
//   Serial.println("IR Receiver Ready");
// }

// void loop() {
//   // --- IRremoteで赤外線データ受信 ---
//   if (IrReceiver.decode()) {
//     uint32_t code = IrReceiver.decodedIRData.decodedRawData;
//     Serial.print("Received IR code: 0x");
//     Serial.println(code, HEX);

//     if (code == 0xF30CFF00) {
//       maindata.Iremotedata = 1;
//     } else if (code == 0xE718FF00) {
//       maindata.Iremotedata = 2;
//     } else if (code == 0xA15EFF00) {
//       maindata.Iremotedata = 3;
//     } else if (code == 0xF708FF00) {
//       maindata.Iremotedata = 4;
//     } 

//     Serial.print("Iremotedata: ");
//     Serial.println(maindata.Iremotedata);

//     IrReceiver.resume();
//   }

//   digitalWrite(SDpin, LOW);

//   Wire.requestFrom(SLAVE_ADDRESS, sizeof(datawithhight));
//   if (Wire.available() >= sizeof(datawithhight)) {
//     Wire.readBytes((uint8_t*)&datawithhight, sizeof(datawithhight));
//     maindata.rudder = datawithhight.rudder;
//     maindata.elevetor = datawithhight.elevetor;
//     maindata.hight = datawithhight.hight;
//   }

//   imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
//   maindata.yaw = euler.x();
//   maindata.pitch = euler.y();
//   maindata.roll = euler.z();
//   maindata.RPM = pulseCount;

//   digitalWrite(4, HIGH);

//   pulseCount = 0;
//   while (gpsSerial.available() > 0) {
//     gps.encode(gpsSerial.read());
//   }

//   if (gps.location.isUpdated()) {
//     maindata.latitude = gps.location.lat();
//     maindata.longitude = gps.location.lng();
//   }
//   if (gps.time.isUpdated()) {
//     maindata.hour = gps.time.hour();
//     maindata.seconds = gps.time.second();
//     maindata.minute = gps.time.minute();
//   }

//   Serial.print("Received rudder: "); Serial.println(datawithhight.rudder);
//   Serial.print("Received elevetor: "); Serial.println(datawithhight.elevetor);
//   Serial.print("Received hight: "); Serial.println(datawithhight.hight);
//   Serial.print("Received roll: "); Serial.println(maindata.roll);
//   Serial.print("Received pitch: "); Serial.println(maindata.pitch);
//   Serial.print("Received yaw: "); Serial.println(maindata.yaw);
//   Serial.print("Received RPM: "); Serial.println(maindata.RPM);
//   Serial.print("Received hour: "); Serial.println(maindata.hour);
//   Serial.print("Received seconds: "); Serial.println(maindata.seconds);
//   Serial.print("Received latitude: "); Serial.println(maindata.latitude, 6);
//   Serial.print("Received longitude: "); Serial.println(maindata.longitude, 6);
//   Serial.print("Received Iremotedata: "); Serial.println(maindata.Iremotedata);

//   File file = SD.open("/data.txt", FILE_APPEND);
//   if (file) {
//     digitalWrite(SDpin, HIGH);
//     file.print("rudder: "); file.println(maindata.rudder);
//     file.print("elevetor: "); file.println(maindata.elevetor);
//     file.print("hight: "); file.println(maindata.hight);
//     file.print("roll: "); file.println(maindata.roll);
//     file.print("pitch: "); file.println(maindata.pitch);
//     file.print("yaw: "); file.println(maindata.yaw);
//     file.print("RPM: "); file.println(maindata.RPM);
//     file.print("hour: "); file.println(maindata.hour);
//     file.print("seconds: "); file.println(maindata.seconds);
//     file.print("latitude: "); file.println(maindata.latitude, 6);
//     file.print("longitude: "); file.println(maindata.longitude, 6);
//     file.print("Iremotedata: "); file.println(maindata.Iremotedata);
//     Serial.println("Data written to SD card");
//     file.close();
//   } else {
//     Serial.println("Error opening data.txt");
//   }

//   Wire.beginTransmission(SLAVE_ADDRESS2);
//   Wire.write((uint8_t*)&maindata, sizeof(maindata));
//   Wire.endTransmission();
//   Serial.println("Data sent to slave!");

//   delay(100);
//   digitalWrite(4, LOW);
// }
























// 実際に使った！！
// 送信されてきたでーたに、高度、GPS、姿勢のデータを統合して、
// 構造体としてSDに保存している


//IRremoteで赤外線データを受信
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <TinyGPSPlus.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <IRremote.h>

int sck = 18;
int miso = 19;
int mosi = 23;
int cs = 5;

struct withHight {
  float rudder = 0;
  float elevetor = 0;
  float hight = 0;
};

// Iremotedataを追加
struct mainData {
  float rudder = 0;
  float elevetor = 0;
  float hight = 0;
  float RPM = 0;
  float roll = 0;
  float pitch = 0;
  float yaw = 0;
  float hour = 0;
  float minute = 0;
  float seconds = 0;
  float latitude = 0;
  float longitude = 0;
  int Iremotedata = 1; // 赤外線リモコンの値を格納
  int flughtnumber = 0; // フライト番号
};

#define SLAVE_ADDRESS 0x08
#define SLAVE_ADDRESS2 0x04
#define SDpin 32
#define IR_RECEIVE_PIN 2  // 赤外線受信モジュールの接続ピン

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);
withHight datawithhight;
mainData maindata;

#define ENCODER_PIN_A 14
#define ENCODER_PIN_B 15

volatile long pulseCount = 0;
unsigned long lastMillis = 0;

#define RXD2 17
#define TXD2 16
#define GPS_BAUD 9600

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

unsigned long lastRpmReset = 0;

void countPulse() {
  if (digitalRead(ENCODER_PIN_B) == HIGH) {
    pulseCount++;
  } else {
    pulseCount--;
  }
}



// ...既存のコード...

// 非同期処理用の変数
volatile bool irReceivedFlag = false;
unsigned long irHighStartTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(SDpin, OUTPUT);
  Wire.begin();
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);

  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), countPulse, RISING);

  pinMode(4, OUTPUT);

  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  SPI.begin(sck, miso, mosi, cs);
  if (!SD.begin(cs)) {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.println("SD Card initialized");

  // IRremote初期化
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR Receiver Ready");

  File file = SD.open("/data.txt", FILE_APPEND);

  if (file) {
    digitalWrite(SDpin, HIGH);
    file.println("millis");
    file.print("rudder: "); 
    file.print("elevetor: "); 
    file.print("hight: "); 
    file.print("roll: "); 
    file.print("pitch: ");
    file.print("yaw: "); 
    file.print("RPM: "); 
    file.print("hour: ");
    file.print("seconds: "); 
    file.print("minute: ");
    file.print("latitude: ");
    file.print("longitude: "); 
    file.print("Iremotedata: "); 
    file.print("flight unmber: "); 
    Serial.println("Data written to SD card");
    file.close();
  } else {
    Serial.println("Error opening data.txt");
  }


}


int prevIremotedata = 1;


void loop() {
  // --- IRremoteで赤外線データ受信 ---
  int nowtime= millis();
  if (IrReceiver.decode()) {
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("Received IR code: 0x");
    Serial.println(code, HEX);

    if (code == 0xF30CFF00) {
      maindata.Iremotedata = 1;
      digitalWrite(4, HIGH);
      irReceivedFlag = true;
      irHighStartTime = millis();
    } else if (code == 0xBA45FF00){
      maindata.Iremotedata = 2;
      digitalWrite(4, HIGH);
      irReceivedFlag = true;
      irHighStartTime = millis();
    } 
    else if (code == 0xA15EFF00) {
      maindata.Iremotedata = 3;
      digitalWrite(4, HIGH);
      irReceivedFlag = true;
      irHighStartTime = millis();
    } 
//     else if (code == 0xF708FF00) {
//       maindata.Iremotedata = 4;
//       digitalWrite(4, HIGH);
//       irReceivedFlag = true;
//       irHighStartTime = millis();
//     }

    Serial.print("Iremotedata: ");
    Serial.println(maindata.Iremotedata);

    IrReceiver.resume();
  }

  // Iremotedataが2→1に変化したらflughtnumberをインクリメント
  if (prevIremotedata == 2 && maindata.Iremotedata == 1) {
    maindata.flughtnumber++;
    Serial.print("flughtnumber incremented: ");
    Serial.println(maindata.flughtnumber);
  }
  prevIremotedata = maindata.Iremotedata;

  // IO4ピンを2秒後にLOWに戻す（非同期処理）
  if (irReceivedFlag && (millis() - irHighStartTime >= 2000)) {
    digitalWrite(4, LOW);
    irReceivedFlag = false;
  }

  // ...既存のloop()処理...
  digitalWrite(SDpin, LOW);

  Wire.requestFrom(SLAVE_ADDRESS, sizeof(datawithhight));
  if (Wire.available() >= sizeof(datawithhight)) {
    Wire.readBytes((uint8_t*)&datawithhight, sizeof(datawithhight));
    maindata.rudder = datawithhight.rudder;
    maindata.elevetor = datawithhight.elevetor;
    maindata.hight = datawithhight.hight;
  }

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  maindata.yaw = euler.x();
  maindata.pitch = euler.y();
  maindata.roll = euler.z();
  // maindata.RPM = pulseCount;

  
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    maindata.latitude = gps.location.lat();
    maindata.longitude = gps.location.lng();
  }
  if (gps.time.isUpdated()) {
    int hour = gps.time.hour();
    int Jhour = hour + 9;
    if(Jhour >=24){
        maindata.hour = Jhour - 24;
    }
    else{
        maindata.hour = Jhour;
    }
    maindata.seconds = gps.time.second();
    maindata.minute = gps.time.minute();
  }

  

  // if(millis()-nowtime > 100){
  //   Serial.println("pulsecount");
  //   Serial.println(pulseCount);
  //   float fusoku = (0.0981*pulseCount+0.9844)*(125/100);
  //   maindata.RPM = fusoku;
  //   pulseCount = 0;
  // }


   if (millis() - lastRpmReset > 100) {
    // Serial.println("pulsecount");
    // Serial.println(pulseCount);
    // float fusoku = (0.0981 * pulseCount + 0.9844) * (125.0 / 123.0);
    // maindata.RPM = fusoku/10;
    // pulseCount = 0;
    // lastRpmReset = millis();
    maindata.RPM = pulseCount; // ここでRPMを更新
    pulseCount = 0;
  }

  Serial.print("Received rudder: "); Serial.println(datawithhight.rudder);
  Serial.print("Received elevetor: "); Serial.println(datawithhight.elevetor);
  Serial.print("Received hight: "); Serial.println(datawithhight.hight);
  Serial.print("Received roll: "); Serial.println(maindata.roll);
  Serial.print("Received pitch: "); Serial.println(maindata.pitch);
  Serial.print("Received yaw: "); Serial.println(maindata.yaw);
  Serial.print("Received RPM: "); Serial.println(maindata.RPM);
  Serial.print("Received hour: "); Serial.println(maindata.hour);
  Serial.print("Received minute: "); Serial.println(maindata.minute);
  Serial.print("Received seconds: "); Serial.println(maindata.seconds);
  Serial.print("Received latitude: "); Serial.println(maindata.latitude, 6);
  Serial.print("Received longitude: "); Serial.println(maindata.longitude, 6);
  Serial.print("Received Iremotedata: "); Serial.println(maindata.Iremotedata);
  Serial.print("flight unmber"); Serial.println(maindata.flughtnumber);

  File file = SD.open("/data.txt", FILE_APPEND);
  if (file) {
    digitalWrite(SDpin, HIGH);
    file.print(millis()); file.print(",");
    // file.print("rudder: "); 
    file.print(maindata.rudder); file.print(",");
    // file.print("elevetor: "); 
    file.print(maindata.elevetor); file.print(",");
    // file.print("hight: "); 
    file.print(maindata.hight); file.print(",");
    // file.print("roll: "); 
    file.print(maindata.roll); file.print(",");
    // file.print("pitch: "); 
    file.print(maindata.pitch); file.print(",");
    // file.print("yaw: "); 
    file.print(maindata.yaw); file.print(",");
    // file.print("RPM: "); 
    file.print(maindata.RPM); file.print(",");
    // file.print("hour: "); 
    file.print(maindata.hour); file.print(",");
    // file.print("seconds: "); 
    file.print(maindata.seconds); file.print(",");
    // file.print("minute: "); 
    file.print(maindata.minute); file.print(",");
    // file.print("latitude: ");
    file.print(maindata.latitude, 6); file.print(",");
    // file.print("longitude: "); 
    file.print(maindata.longitude, 6); file.print(",");
    // file.print("Iremotedata: "); 
    file.print(maindata.Iremotedata); file.print(",");
    // file.print("flight unmber: "); 
    file.println(maindata.flughtnumber);
    Serial.println("Data written to SD card");
    file.close();
  } else {
    Serial.println("Error opening data.txt");
  }

  Wire.beginTransmission(SLAVE_ADDRESS2);
  Wire.write((uint8_t*)&maindata, sizeof(maindata));
  Wire.endTransmission();
  Serial.println("Data sent to slave!");

  // maindata.RPM=0;

  delay(100);
}



























// //  メイン基板から送信されてきたデータをI2Cで受信する
// #include <Arduino.h>
// #include <Wire.h>

// #define SLAVE_ADDRESS2 0x04 // スレーブアドレス

// // 受信するデータ構造体
// struct mainData {
//   float rudder;
//   float elevetor;
//   float hight;
//   float RPM;
//   float roll;
//   float pitch;
//   float yaw;
//   float hour;  //年、月、日付、時間、分、秒を取得
//   float seconds;
//   float latitude;
//   float longitude;
// };

// mainData receivedData; // 受信データ格納用

// // I2Cデータ受信時の処理
// void receiveEvent(int numBytes) {
//     if (numBytes >= sizeof(mainData)) { // 必要サイズ以上のデータを受信したか確認
//         Wire.readBytes((uint8_t*)&receivedData, sizeof(receivedData));

//         // 受信データをシリアルモニターに表示
//         Serial.println("=== Received Data ===");
//         Serial.print("Rudder: "); Serial.println(receivedData.rudder);
//         Serial.print("Elevetor: "); Serial.println(receivedData.elevetor);
//         Serial.print("Hight: "); Serial.println(receivedData.hight);
//         Serial.print("Roll: "); Serial.println(receivedData.roll);
//         Serial.print("Pitch: "); Serial.println(receivedData.pitch);
//         Serial.print("Yaw: "); Serial.println(receivedData.yaw);
//         Serial.print("RPM: "); Serial.println(receivedData.RPM);
//         Serial.print("Hour: "); Serial.println(receivedData.hour);
//         Serial.print("Seconds: "); Serial.println(receivedData.seconds);
//         Serial.print("Latitude: "); Serial.println(receivedData.latitude, 6);
//         Serial.print("Longitude: "); Serial.println(receivedData.longitude, 6);
//     } else {
//         Serial.println("Error: Incomplete data received.");
//     }
// }

// void setup() {
//     Wire.begin(SLAVE_ADDRESS2); // スレーブとして開始
//     Wire.onReceive(receiveEvent); // I2C受信時のコールバック設定
//     Serial.begin(115200);
//     Serial.println("I2C Slave Ready. Waiting for data...");
// }

// void loop() {
//     delay(100); // 受信データを表示する間隔
// }



















// // 　I2Cで受信したデータをfirebaseに送信するコード
// #include <Arduino.h>
// #include <Wire.h>
// #if defined(ESP32)
//   #include <WiFi.h>
// #elif defined(ESP8266)
//   #include <ESP8266WiFi.h>
// #endif
// #include <Firebase_ESP_Client.h>

// // Provide the token generation process info.
// #include "addons/TokenHelper.h"
// // Provide the RTDB payload printing info and other helper functions.
// #include "addons/RTDBHelper.h"

// // Insert your network credentials
// #define WIFI_SSID "moto g(30)"
// #define WIFI_PASSWORD "unchidao"

// // Insert Firebase project API Key
// #define API_KEY "AIzaSyCmdsn2pNkiZe7cAeiXlqb9ziCrJl3tQ1A"

// // Insert RTDB URL
// #define DATABASE_URL "https://esp32-4-27-default-rtdb.asia-southeast1.firebasedatabase.app/"

// // Define Firebase Data object
// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// #define firebasepin 4

// #define SLAVE_ADDRESS2 0x04 // I2Cスレーブアドレス

// // 受信するデータ構造体
// struct mainData {
//   float rudder;
//   float elevetor;
//   float hight;
//   float RPM;
//   float roll;
//   float pitch;
//   float yaw;
//   float hour;
//   float seconds;
//   float latitude;
//   float longitude;
// };

// mainData receivedData; // 受信データ格納用
// bool signupOK = false;
// unsigned long lastSendTime = 0; // 最後に送信した時間

// // I2Cデータ受信時の処理
// void receiveEvent(int numBytes) {
//     if (numBytes >= sizeof(mainData)) {
//         Wire.readBytes((uint8_t*)&receivedData, sizeof(receivedData));

//         // 受信データをシリアルモニターに表示
//         Serial.println("=== Received Data ===");
//         Serial.print("Rudder: "); Serial.println(receivedData.rudder);
//         Serial.print("Elevetor: "); Serial.println(receivedData.elevetor);
//         Serial.print("Hight: "); Serial.println(receivedData.hight);
//         Serial.print("Roll: "); Serial.println(receivedData.roll);
//         Serial.print("Pitch: "); Serial.println(receivedData.pitch);
//         Serial.print("Yaw: "); Serial.println(receivedData.yaw);
//         Serial.print("RPM: "); Serial.println(receivedData.RPM);
//         Serial.print("Hour: "); Serial.println(receivedData.hour);
//         Serial.print("Seconds: "); Serial.println(receivedData.seconds);
//         Serial.print("Latitude: "); Serial.println(receivedData.latitude, 6);
//         Serial.print("Longitude: "); Serial.println(receivedData.longitude, 6);
//     } else {
//         Serial.println("Error: Incomplete data received.");
//     }
// }


// void sendToFirebase() {
//     if (Firebase.ready() && signupOK) {
//         FirebaseJson json;
//         json.set("rudder", receivedData.rudder);
//         json.set("elevetor", receivedData.elevetor);
//         json.set("hight", receivedData.hight);
//         json.set("roll", receivedData.roll);
//         json.set("pitch", receivedData.pitch);
//         json.set("yaw", receivedData.yaw);
//         json.set("RPM", receivedData.RPM);
//         json.set("hour", receivedData.hour);
//         json.set("seconds", receivedData.seconds);
//         json.set("latitude", receivedData.latitude);
//         json.set("longitude", receivedData.longitude);

//         if (Firebase.RTDB.setJSON(&fbdo, "receivedData", &json)) {
//             Serial.println("Data sent to Firebase successfully.");
//             digitalWrite(firebasepin, HIGH); // SDpinをHIGHに設定
//         } else {
//             Serial.println("Failed to send data to Firebase.");
//             Serial.println("Reason: " + fbdo.errorReason());
//         }
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     Wire.begin(SLAVE_ADDRESS2); // スレーブとして開始
//     Wire.onReceive(receiveEvent); // I2C受信時のコールバック設定
//     Serial.println("I2C Slave Ready. Waiting for data...");

//     // - SDpin の初期設定
//     pinMode(firebasepin, OUTPUT); 


//     // Wi-Fi接続
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     Serial.print("Connecting to Wi-Fi");
//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print(".");
//         delay(300);
//     }
//     Serial.println();
//     Serial.print("Connected with IP: ");
//     Serial.println(WiFi.localIP());

//     // Firebase設定
//     config.api_key = API_KEY;
//     config.database_url = DATABASE_URL;
//     config.token_status_callback = tokenStatusCallback;

//     if (Firebase.signUp(&config, &auth, "", "")) {
//         Serial.println("Firebase SignUp OK");
//         signupOK = true;
//     } else {
//         Serial.printf("Firebase SignUp FAILED: %s\n", config.signer.signupError.message.c_str());
//     }

//     Firebase.begin(&config, &auth);
//     Firebase.reconnectWiFi(true);
// }

// void loop() {
//     unsigned long currentMillis = millis();
//     digitalWrite(firebasepin, LOW); // SDpinをLOWに設定
//     if (currentMillis - lastSendTime >= 500) {
//         lastSendTime = currentMillis;
//         sendToFirebase();
//     }
// }

























// // // 　I2Cで受信したデータをfirebaseに送信し、
// // // 1. 最新値を常に上書きするノード（例: "receivedData/latest"）
// // // 2. タイムスタンプ付きで履歴を追加するノード（例: "receivedData/history"）
// // // の両方に保存するコード
// #include <Arduino.h>
// #include <Wire.h>
// #if defined(ESP32)
//   #include <WiFi.h>
// #elif defined(ESP8266)
//   #include <ESP8266WiFi.h>
// #endif
// #include <Firebase_ESP_Client.h>
// #include "addons/TokenHelper.h"
// #include "addons/RTDBHelper.h"

// #define WIFI_SSID "moto g(30)"
// #define WIFI_PASSWORD "unchidao"
// #define API_KEY "AIzaSyCmdsn2pNkiZe7cAeiXlqb9ziCrJl3tQ1A"
// #define DATABASE_URL "https://esp32-4-27-default-rtdb.asia-southeast1.firebasedatabase.app/"

// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// #define firebasepin 4
// #define SLAVE_ADDRESS2 0x04 // I2Cスレーブアドレス

// // 受信するデータ構造体（送信側と同じ定義にすること）
// struct mainData {
//   float rudder;
//   float elevetor;
//   float hight;
//   float RPM;
//   float roll;
//   float pitch;
//   float yaw;
//   float hour;
//   float minute;
//   float seconds;
//   float latitude;
//   float longitude;
//   int Iremotedata;
//   int flughtnumber;
// };

// mainData receivedData;
// bool signupOK = false;
// unsigned long lastSendTime = 0;

// // I2Cデータ受信時の処理
// void receiveEvent(int numBytes) {
//     if (numBytes >= sizeof(mainData)) {
//         Wire.readBytes((uint8_t*)&receivedData, sizeof(receivedData));
//         Serial.println("=== Received Data ===");
//         Serial.print("Rudder: "); Serial.println(receivedData.rudder);
//         Serial.print("Elevetor: "); Serial.println(receivedData.elevetor);
//         Serial.print("Hight: "); Serial.println(receivedData.hight);
//         Serial.print("Roll: "); Serial.println(receivedData.roll);
//         Serial.print("Pitch: "); Serial.println(receivedData.pitch);
//         Serial.print("Yaw: "); Serial.println(receivedData.yaw);
//         Serial.print("RPM: "); Serial.println(receivedData.RPM);
//         Serial.print("Hour: "); Serial.println(receivedData.hour);
//         Serial.print("Minute: "); Serial.println(receivedData.minute);
//         Serial.print("Seconds: "); Serial.println(receivedData.seconds);
//         Serial.print("Latitude: "); Serial.println(receivedData.latitude, 6);
//         Serial.print("Longitude: "); Serial.println(receivedData.longitude, 6);
//         Serial.print("Iremotedata: "); Serial.println(receivedData.Iremotedata);
//         Serial.print("flughtnumber: "); Serial.println(receivedData.flughtnumber);
//     } else {
//         Serial.println("Error: Incomplete data received.");
//     }
// }

// void sendToFirebase() {
//     if (Firebase.ready() && signupOK) {
//         FirebaseJson json;
//         json.set("rudder", receivedData.rudder);
//         json.set("elevetor", receivedData.elevetor);
//         json.set("hight", receivedData.hight);
//         json.set("RPM", receivedData.RPM);
//         json.set("roll", receivedData.roll);
//         json.set("pitch", receivedData.pitch);
//         json.set("yaw", receivedData.yaw);
//         json.set("hour", receivedData.hour);
//         json.set("minute", receivedData.minute);
//         json.set("seconds", receivedData.seconds);
//         json.set("latitude", receivedData.latitude);
//         json.set("longitude", receivedData.longitude);
//         json.set("Iremotedata", receivedData.Iremotedata);
//         json.set("flughtnumber", receivedData.flughtnumber);

//         // 1. 最新値ノード（上書き）
//         if (Firebase.RTDB.setJSON(&fbdo, "receivedData/latest", &json)) {
//             Serial.println("Latest data sent to Firebase successfully.");
//         } else {
//             Serial.println("Failed to send latest data to Firebase.");
//             Serial.println("Reason: " + fbdo.errorReason());
//         }

//         // 2. 履歴ノード（タイムスタンプ付きでpush）
//         String path = "receivedData/history";
//         json.set("timestamp", (int)time(nullptr)); // UNIXタイムスタンプを追加
//         if (Firebase.RTDB.pushJSON(&fbdo, path.c_str(), &json)) {
//             Serial.println("History data pushed to Firebase successfully.");
//         } else {
//             Serial.println("Failed to push history data to Firebase.");
//             Serial.println("Reason: " + fbdo.errorReason());
//         }
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     Wire.begin(SLAVE_ADDRESS2);
//     Wire.onReceive(receiveEvent);
//     Serial.println("I2C Slave Ready. Waiting for data...");

//     pinMode(firebasepin, OUTPUT);

//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     Serial.print("Connecting to Wi-Fi");
//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print(".");
//         delay(300);
//     }
//     Serial.println();
//     Serial.print("Connected with IP: ");
//     Serial.println(WiFi.localIP());

//     config.api_key = API_KEY;
//     config.database_url = DATABASE_URL;
//     config.token_status_callback = tokenStatusCallback;

//     if (Firebase.signUp(&config, &auth, "", "")) {
//         Serial.println("Firebase SignUp OK");
//         signupOK = true;
//     } else {
//         Serial.printf("Firebase SignUp FAILED: %s\n", config.signer.signupError.message.c_str());
//     }

//     Firebase.begin(&config, &auth);
//     Firebase.reconnectWiFi(true);

//     // NTPで時刻を取得（タイムスタンプ用）
//     configTime(9 * 3600, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
// }

// void loop() {
//     unsigned long currentMillis = millis();
//     digitalWrite(firebasepin, LOW);
//     if (currentMillis - lastSendTime >= 500) {
//         lastSendTime = currentMillis;
//         sendToFirebase();
//     }
// }



















// // // 　I2Cで受信したデータをfirebaseに送信し、
// // // 1. 最新値を常に上書きするノード（例: "receivedData/latest"）
// // // 2. タイムスタンプ付きで履歴を追加するノード（例: "receivedData/history"）
// // // の両方に保存するコード
// #include <Arduino.h>
// #include <Wire.h>
// #if defined(ESP32)
//   #include <WiFi.h>
// #elif defined(ESP8266)
//   #include <ESP8266WiFi.h>
// #endif
// #include <Firebase_ESP_Client.h>
// #include "addons/TokenHelper.h"
// #include "addons/RTDBHelper.h"

// #define WIFI_SSID "moto g(30)"
// #define WIFI_PASSWORD "unchidao"
// #define API_KEY "AIzaSyDmCxYyhBU2kHAASlDs5IhbH_ojjhdjTC4"
// #define DATABASE_URL "https://esp32-5-27-default-rtdb.asia-southeast1.firebasedatabase.app/"

// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// #define firebasepin 4
// #define SLAVE_ADDRESS2 0x04 // I2Cスレーブアドレス

// // 受信するデータ構造体（送信側と同じ定義にすること）
// struct mainData {
//   float rudder;
//   float elevetor;
//   float hight;
//   float RPM;
//   float roll;
//   float pitch;
//   float yaw;
//   float hour;
//   float minute;
//   float seconds;
//   float latitude;
//   float longitude;
//   int Iremotedata;
//   int flughtnumber;
// };

// // ...既存のインクルード・定義...

// // ...既存のインクルード・定義...

// mainData receivedData;
// bool signupOK = false;
// unsigned long lastSendTime = 0;

// // 追加：記録制御用フラグとカウンタ
// bool isRecording = false;
// int historyIndex = 1; // タイムスタンプ代用カウンタ
// static int lastFlughtnumber = -1;

// // I2Cデータ受信時の処理
// void receiveEvent(int numBytes) {
//     if (numBytes >= sizeof(mainData)) {
//         Wire.readBytes((uint8_t*)&receivedData, sizeof(receivedData));
//         Serial.println("=== Received Data ===");
//         Serial.print("Rudder: "); Serial.println(receivedData.rudder);
//         Serial.print("Elevetor: "); Serial.println(receivedData.elevetor);
//         Serial.print("Hight: "); Serial.println(receivedData.hight);
//         Serial.print("Roll: "); Serial.println(receivedData.roll);
//         Serial.print("Pitch: "); Serial.println(receivedData.pitch);
//         Serial.print("Yaw: "); Serial.println(receivedData.yaw);
//         Serial.print("RPM: "); Serial.println(receivedData.RPM);
//         Serial.print("Hour: "); Serial.println(receivedData.hour);
//         Serial.print("Minute: "); Serial.println(receivedData.minute);
//         Serial.print("Seconds: "); Serial.println(receivedData.seconds);
//         Serial.print("Latitude: "); Serial.println(receivedData.latitude, 6);
//         Serial.print("Longitude: "); Serial.println(receivedData.longitude, 6);
//         Serial.print("Iremotedata: "); Serial.println(receivedData.Iremotedata);
//         Serial.print("flughtnumber: "); Serial.println(receivedData.flughtnumber);
//     } else {
//         Serial.println("Error: Incomplete data received.");
//     }
// }

// void sendToFirebase() {
//     if (Firebase.ready() && signupOK) {
//         // latestノードには常に上書き
//         FirebaseJson json;
//         json.set("rudder", receivedData.rudder);
//         json.set("elevetor", receivedData.elevetor);
//         json.set("hight", receivedData.hight);
//         json.set("RPM", receivedData.RPM);
//         json.set("roll", receivedData.roll);
//         json.set("pitch", receivedData.pitch);
//         json.set("yaw", receivedData.yaw);
//         json.set("hour", receivedData.hour);
//         json.set("minute", receivedData.minute);
//         json.set("seconds", receivedData.seconds);
//         json.set("latitude", receivedData.latitude);
//         json.set("longitude", receivedData.longitude);
//         json.set("Iremotedata", receivedData.Iremotedata);
//         json.set("flughtnumber", receivedData.flughtnumber);

//         if (Firebase.RTDB.setJSON(&fbdo, "receivedData/latest", &json)) {
//             Serial.println("Latest data sent to Firebase successfully.");
//         } else {
//             Serial.println("Failed to send latest data to Firebase.");
//             Serial.println("Reason: " + fbdo.errorReason());
//         }

//         // historyノードへの記録制御
//         static int lastIremoteData = 1;

//         if (receivedData.Iremotedata == 2) {
//             // 新しいflughtnumberになったらカウンタをリセット
//             if (!isRecording || lastFlughtnumber != receivedData.flughtnumber) {
//                 isRecording = true;
//                 lastFlughtnumber = receivedData.flughtnumber;
//                 historyIndex = 1;
//                 Serial.print("Start recording for flughtnumber: ");
//                 Serial.println(lastFlughtnumber);
//             }
//             // タイムスタンプの代わりにhistoryIndexを使う
//             String path = "receivedData/history/flught" + String(receivedData.flughtnumber) + "/" + String(historyIndex);

//             FirebaseJson histJson;
//             histJson.set("rudder", receivedData.rudder);
//             histJson.set("elevetor", receivedData.elevetor);
//             histJson.set("hight", receivedData.hight);
//             histJson.set("RPM", receivedData.RPM);
//             histJson.set("roll", receivedData.roll);
//             histJson.set("pitch", receivedData.pitch);
//             histJson.set("yaw", receivedData.yaw);
//             histJson.set("hour", receivedData.hour);
//             histJson.set("minute", receivedData.minute);
//             histJson.set("seconds", receivedData.seconds);
//             histJson.set("latitude", receivedData.latitude);
//             histJson.set("longitude", receivedData.longitude);
//             histJson.set("Iremotedata", receivedData.Iremotedata);
//             histJson.set("flughtnumber", receivedData.flughtnumber);

//             if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &histJson)) {
//                 Serial.print("History data saved to Firebase. Index: ");
//                 Serial.println(historyIndex);
//                 historyIndex++; // 次の記録で+1
//             } else {
//                 Serial.println("Failed to save history data to Firebase.");
//                 Serial.println("Reason: " + fbdo.errorReason());
//             }
//         } else if (receivedData.Iremotedata == 1 && lastIremoteData == 2) {
//             // 記録終了
//             isRecording = false;
//             Serial.println("Stop recording.");
//         }
//         lastIremoteData = receivedData.Iremotedata;
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     Wire.begin(SLAVE_ADDRESS2);
//     Wire.onReceive(receiveEvent);
//     Serial.println("I2C Slave Ready. Waiting for data...");

//     pinMode(firebasepin, OUTPUT);

//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     Serial.print("Connecting to Wi-Fi");
//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print(".");
//         delay(300);
//     }
//     Serial.println();
//     Serial.print("Connected with IP: ");
//     Serial.println(WiFi.localIP());

//     config.api_key = API_KEY;
//     config.database_url = DATABASE_URL;
//     config.token_status_callback = tokenStatusCallback;

//     if (Firebase.signUp(&config, &auth, "", "")) {
//         Serial.println("Firebase SignUp OK");
//         signupOK = true;
//     } else {
//         Serial.printf("Firebase SignUp FAILED: %s\n", config.signer.signupError.message.c_str());
//     }

//     Firebase.begin(&config, &auth);
//     Firebase.reconnectWiFi(true);

//     // NTPで時刻を取得（タイムスタンプ用）
//     configTime(9 * 3600, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
// }

// void loop() {
//     unsigned long currentMillis = millis();
//     digitalWrite(firebasepin, LOW);
//     if (currentMillis - lastSendTime >= 500) {
//         lastSendTime = currentMillis;
//         sendToFirebase();
//     }
// }



















// 実際に使った！！
// メイン処理をしているマイコンからI2Cで受信したデータをFirebaseに送信するコード

// // // 　I2Cで受信したデータをfirebaseに送信し、
// // // 1. 最新値を常に上書きするノード（例: "receivedData/latest"）
// // // 2. タイムスタンプ付きで履歴を追加するノード（例: "receivedData/history"）
// // // の両方に保存するコード
// #include <Arduino.h>
// #include <Wire.h>
// #if defined(ESP32)
//   #include <WiFi.h>
// #elif defined(ESP8266)
//   #include <ESP8266WiFi.h>
// #endif
// #include <Firebase_ESP_Client.h>
// #include "addons/TokenHelper.h"
// #include "addons/RTDBHelper.h"

// #define WIFI_SSID "moto g(30)"
// #define WIFI_PASSWORD "unchidao"
// #define API_KEY "AIzaSyDPzzhLFurxykUfQm4GBs7YLJmpbY7spik"
// #define DATABASE_URL "https://fir-7-08-default-rtdb.asia-southeast1.firebasedatabase.app/"

// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// #define firebasepin 4
// #define SLAVE_ADDRESS2 0x04 // I2Cスレーブアドレス

// // 受信するデータ構造体（送信側と同じ定義にすること）
// struct mainData {
//   float rudder;
//   float elevetor;
//   float hight;
//   float RPM;
//   float roll;
//   float pitch;
//   float yaw;
//   float hour;
//   float minute;
//   float seconds;
//   float latitude;
//   float longitude;
//   int Iremotedata;
//   int flughtnumber;
// };

// // ...既存のインクルード・定義...

// // ...既存のインクルード・定義...

// mainData receivedData;
// bool signupOK = false;
// unsigned long lastSendTime = 0;

// // 追加：記録制御用フラグとカウンタ
// bool isRecording = false;
// int historyIndex = 1; // タイムスタンプ代用カウンタ
// static int lastFlughtnumber = -1;

// // I2Cデータ受信時の処理
// void receiveEvent(int numBytes) {
//     if (numBytes >= sizeof(mainData)) {
//         Wire.readBytes((uint8_t*)&receivedData, sizeof(receivedData));
//         Serial.println("=== Received Data ===");
//         Serial.print("Rudder: "); Serial.println(receivedData.rudder);
//         Serial.print("Elevetor: "); Serial.println(receivedData.elevetor);
//         Serial.print("Hight: "); Serial.println(receivedData.hight);
//         Serial.print("Roll: "); Serial.println(receivedData.roll);
//         Serial.print("Pitch: "); Serial.println(receivedData.pitch);
//         Serial.print("Yaw: "); Serial.println(receivedData.yaw);
//         Serial.print("RPM: "); Serial.println(receivedData.RPM);
//         Serial.print("Hour: "); Serial.println(receivedData.hour);
//         Serial.print("Minute: "); Serial.println(receivedData.minute);
//         Serial.print("Seconds: "); Serial.println(receivedData.seconds);
//         Serial.print("Latitude: "); Serial.println(receivedData.latitude, 6);
//         Serial.print("Longitude: "); Serial.println(receivedData.longitude, 6);
//         Serial.print("Iremotedata: "); Serial.println(receivedData.Iremotedata);
//         Serial.print("flughtnumber: "); Serial.println(receivedData.flughtnumber);
//     } else {
//         Serial.println("Error: Incomplete data received.");
//     }
// }

// // ...既存のコード...
// // ...既存のコード...

// void sendToFirebase() {
//     if (Firebase.ready() && signupOK) {
//         // latestノードには常に上書き
//         FirebaseJson json;
//         json.set("rudder", receivedData.rudder);
//         json.set("elevetor", receivedData.elevetor);
//         json.set("hight", receivedData.hight);
//         json.set("RPM", receivedData.RPM);
//         json.set("roll", receivedData.roll);
//         json.set("pitch", receivedData.pitch);
//         json.set("yaw", receivedData.yaw);
//         json.set("hour", receivedData.hour);
//         json.set("minute", receivedData.minute);
//         json.set("seconds", receivedData.seconds);
//         json.set("latitude", receivedData.latitude);
//         json.set("longitude", receivedData.longitude);
//         json.set("Iremotedata", receivedData.Iremotedata);
//         json.set("flughtnumber", receivedData.flughtnumber);

//         if (Firebase.RTDB.setJSON(&fbdo, "receivedData/latest", &json)) {
//             Serial.println("Latest data sent to Firebase successfully.");
//         } else {
//             Serial.println("Failed to send latest data to Firebase.");
//             Serial.println("Reason: " + fbdo.errorReason());
//         }

//         // historyノードへの記録制御
//         static int lastIremoteData = 1;

//         // ★ Iremotedataが3のときはtestノードに保存
//         if (receivedData.Iremotedata == 3) {
//             String testPath = "test/flught" + String(receivedData.flughtnumber) + "/" + String(historyIndex);
//             if (Firebase.RTDB.setJSON(&fbdo, testPath.c_str(), &json)) {
//                 Serial.print("Test data saved to Firebase. Index: ");
//                 Serial.println(historyIndex);
//                 historyIndex++;
//             } else {
//                 Serial.println("Failed to save test data to Firebase.");
//                 Serial.println("Reason: " + fbdo.errorReason());
//             }
//         }
//         // 通常の履歴保存
//         else if (receivedData.Iremotedata == 2) {
//             if (!isRecording || lastFlughtnumber != receivedData.flughtnumber) {
//                 isRecording = true;
//                 lastFlughtnumber = receivedData.flughtnumber;
//                 historyIndex = 1;
//                 Serial.print("Start recording for flughtnumber: ");
//                 Serial.println(lastFlughtnumber);
//             }
//             String path = "receivedData/history/flught" + String(receivedData.flughtnumber) + "/" + String(historyIndex);

//             FirebaseJson histJson = json;
//             if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &histJson)) {
//                 Serial.print("History data saved to Firebase. Index: ");
//                 Serial.println(historyIndex);
//                 historyIndex++;
//             } else {
//                 Serial.println("Failed to save history data to Firebase.");
//                 Serial.println("Reason: " + fbdo.errorReason());
//             }
//         } else if (receivedData.Iremotedata == 1 && lastIremoteData == 2) {
//             isRecording = false;
//             Serial.println("Stop recording.");
//         }
//         lastIremoteData = receivedData.Iremotedata;
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     Wire.begin(SLAVE_ADDRESS2);
//     Wire.onReceive(receiveEvent);
//     Serial.println("I2C Slave Ready. Waiting for data...");

//     pinMode(firebasepin, OUTPUT);

//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     Serial.print("Connecting to Wi-Fi");
//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print(".");
//         delay(300);
//     }
//     Serial.println();
//     Serial.print("Connected with IP: ");
//     Serial.println(WiFi.localIP());

//     config.api_key = API_KEY;
//     config.database_url = DATABASE_URL;
//     config.token_status_callback = tokenStatusCallback;

//     if (Firebase.signUp(&config, &auth, "", "")) {
//         Serial.println("Firebase SignUp OK");
//         signupOK = true;
//     } else {
//         Serial.printf("Firebase SignUp FAILED: %s\n", config.signer.signupError.message.c_str());
//     }

//     Firebase.begin(&config, &auth);
//     Firebase.reconnectWiFi(true);

//     // NTPで時刻を取得（タイムスタンプ用）
//     configTime(9 * 3600, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
// }

// void loop() {
//     unsigned long currentMillis = millis();
//     digitalWrite(firebasepin, LOW);
//     if (currentMillis - lastSendTime >= 500) {
//         lastSendTime = currentMillis;
//         sendToFirebase();
//     }
// }































// // 以下は特に関係のないコード






// #include <Arduino.h>
// #define ENCODER_PIN_A 14
// #define ENCODER_PIN_B 15

// volatile long pulseCount = 0;
// unsigned long lastMillis = 0;


// void countPulse() {
//   if (digitalRead(ENCODER_PIN_B) == HIGH) {
//     pulseCount++;
//   } else {
//     pulseCount--;
//   }
// }




// void setup() {
//   pinMode(ENCODER_PIN_A, INPUT);
//   pinMode(ENCODER_PIN_B, INPUT);
//   attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), countPulse, RISING);

//   Serial.begin(9600);
//   Serial.println("Hello");

  
// }

// void loop() {
//   if (millis() - lastMillis >= 100) {
//     Serial.print("RPM: ");
//     Serial.println((pulseCount * 60.0) / 1000.0); // 1秒あたりの回転数を表示
//     pulseCount = 0;
//     lastMillis = millis();
//   }
// } 

 








//①文字列化したJSONにしたらよい。





// #include <Arduino.h>
// #include <Wire.h>

// #define SLAVE_ADDRESS2 0x09 // スレーブ2のアドレス

// struct mainData {
//     float rudder;
//     float elevetor;
//     float hight;
//     float RPM;
//     float roll;
//     float pitch;
//     float yaw;
//     float hour;
//     float seconds;
//     float latitude;
//     float longitude;
// };

// mainData receivedData;

// void receiveEvent(int numBytes) {
//     Serial.println("receiveEvent called");
//     if (numBytes >= sizeof(mainData)) {
//         Wire.readBytes((uint8_t*)&receivedData, sizeof(receivedData));
//         Serial.println("Data received successfully");
//         Serial.print("Rudder: "); Serial.println(receivedData.rudder);
//         Serial.print("Elevetor: "); Serial.println(receivedData.elevetor);
//         Serial.print("Hight: "); Serial.println(receivedData.hight);
//         Serial.print("Roll: "); Serial.println(receivedData.roll);
//         Serial.print("Pitch: "); Serial.println(receivedData.pitch);
//         Serial.print("Yaw: "); Serial.println(receivedData.yaw);
//         Serial.print("RPM: "); Serial.println(receivedData.RPM);
//         Serial.print("Hour: "); Serial.println(receivedData.hour);
//         Serial.print("Seconds: "); Serial.println(receivedData.seconds);
//         Serial.print("Latitude: "); Serial.println(receivedData.latitude, 6);
//         Serial.print("Longitude: "); Serial.println(receivedData.longitude, 6);
//     } else {
//         Serial.println("Error: Incomplete data received.");
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     Wire.begin(SLAVE_ADDRESS2); // スレーブとして初期化
//     Wire.onReceive(receiveEvent); // データ受信時のコールバック設定
//     Serial.println("I2C Slave 2 Ready");
// }

// void loop() {
//     delay(100); // メインループは空でも問題ありません
// }











// //ソフトウェアシリアル送信成功　
// #include <Arduino.h>
// #include <SoftwareSerial.h>

// // ソフトウェアシリアルのピン設定
// #define TX_PIN 25 // 送信ピン
// #define RX_PIN 26 // 受信ピン（未使用）

// SoftwareSerial mySerial(RX_PIN, TX_PIN); // RX, TX

// void setup() {
//   Serial.begin(115200); // デバッグ用シリアル通信
//   mySerial.begin(9600); // ソフトウェアシリアル通信
//   Serial.println("ESP32 Sender Ready");
// }

// void loop() {
//   String message = "Hello, Receiver!"; // 送信するメッセージ
//   mySerial.println(message); // メッセージを送信
//   Serial.println("Sent: " + message); // デバッグ用に送信内容を表示
//   delay(1000); // 1秒間隔で送信
// }







// //ソフトウェアシリアル受信成功
// #include <Arduino.h>
// #include <SoftwareSerial.h>

// // ソフトウェアシリアルのピン設定
// #define TX_PIN 25 // 送信ピン（未使用）
// #define RX_PIN 26 // 受信ピン

// SoftwareSerial mySerial(RX_PIN, TX_PIN); // RX, TX

// void setup() {
//   Serial.begin(115200); // デバッグ用シリアル通信
//   mySerial.begin(9600); // ソフトウェアシリアル通信
//   Serial.println("ESP32 Receiver Ready");
// }

// void loop() {
//   if (mySerial.available()) { // データが受信された場合
//     String receivedMessage = mySerial.readStringUntil('\n'); // 改行までの文字列を読み取る
//     Serial.println("Received: " + receivedMessage); // 受信したメッセージを表示
//   }
// }


















// //　UART2でGPSデータを受信して、SoftwareSerialを使って別のESP32に送信
// #include <Arduino.h>
// #include <TinyGPSPlus.h>
// #include <SoftwareSerial.h>

// // UART2のピン設定 (TinyGPS用)
// #define RX_PIN_UART2 16 // UART2受信ピン (GPSモジュールのTX)
// #define TX_PIN_UART2 17 // UART2送信ピン (未使用)

// // SoftwareSerialのピン設定 (別のESP32への送信用)
// #define TX_PIN_SOFT 25 // SoftwareSerial送信ピン
// #define RX_PIN_SOFT 26 // SoftwareSerial受信ピン（未使用）

// TinyGPSPlus gps; // TinyGPSPlusオブジェクト
// SoftwareSerial mySerial(RX_PIN_SOFT, TX_PIN_SOFT); // SoftwareSerialのRX, TX

// void setup() {
//   // デバッグ用シリアル通信
//   Serial.begin(115200);
//   Serial.println("ESP32 GPS Sender Ready");

//   // UART2の初期化 (TinyGPS用)
//   Serial2.begin(9600, SERIAL_8N1, RX_PIN_UART2, TX_PIN_UART2);
//   Serial.println("UART2 Ready for GPS");

//   // SoftwareSerialの初期化
//   mySerial.begin(9600);
//   Serial.println("SoftwareSerial Ready for ESP32 Communication");
// }

// void loop() {
//   // GPSデータの受信
//   while (Serial2.available() > 0) {
//     char c = Serial2.read();
//     gps.encode(c); // TinyGPSにデータを渡す

//     // GPSデータが更新された場合
//     if (gps.location.isUpdated()) {
//       String gpsData = "Lat: " + String(gps.location.lat(), 6) + ", Lon: " + String(gps.location.lng(), 6);
//       Serial.println("GPS Data: " + gpsData);

//       // SoftwareSerialで別のESP32に送信
//       mySerial.println(gpsData);
//       Serial.println("Sent via SoftwareSerial: " + gpsData);
//     }
//   }

//   delay(100); // 少し待機
// }





// //SoftwareSerialを使って送信されたデータを受信した
// #include <Arduino.h>
// #include <SoftwareSerial.h>

// // SoftwareSerialのピン設定 (送信側ESP32からの受信用)
// #define TX_PIN_SOFT 25 // SoftwareSerial送信ピン（未使用）
// #define RX_PIN_SOFT 26 // SoftwareSerial受信ピン

// SoftwareSerial mySerial(RX_PIN_SOFT, TX_PIN_SOFT); // SoftwareSerialのRX, TX

// void setup() {
//   // デバッグ用シリアル通信
//   Serial.begin(115200);
//   Serial.println("ESP32 GPS Receiver Ready");

//   // SoftwareSerialの初期化
//   mySerial.begin(9600);
//   Serial.println("SoftwareSerial Ready for ESP32 Communication");
// }

// void loop() {
//   // SoftwareSerialでデータを受信
//   if (mySerial.available()) {
//     String receivedData = mySerial.readStringUntil('\n'); // 改行までの文字列を読み取る
//     Serial.println("Received via SoftwareSerial: " + receivedData); // 受信データを表示
//   }

//   delay(100); // 少し待機
// }










// #include <Arduino.h>
// #include <TinyGPSPlus.h>
// #include <SoftwareSerial.h>

// // UART2のピン設定 (TinyGPS用)
// #define RX_PIN_UART2 16 // UART2受信ピン (GPSモジュールのTX)
// #define TX_PIN_UART2 17 // UART2送信ピン (未使用)

// // SoftwareSerialのピン設定 (別のESP32への送信用)
// #define TX_PIN_SOFT 25 // SoftwareSerial送信ピン
// #define RX_PIN_SOFT 26 // SoftwareSerial受信ピン（未使用）

// TinyGPSPlus gps; // TinyGPSPlusオブジェクト
// SoftwareSerial mySerial(RX_PIN_SOFT, TX_PIN_SOFT); // SoftwareSerialのRX, TX

// // GPSデータを格納する構造体
// struct GPSData {
//   float latitude;
//   float longitude;
//   int randomValue; // ランダムな値を保持する変数
// };

// void setup() {
//   // デバッグ用シリアル通信
//   Serial.begin(115200);
//   Serial.println("ESP32 GPS Sender Ready");

//   // UART2の初期化 (TinyGPS用)
//   Serial2.begin(9600, SERIAL_8N1, RX_PIN_UART2, TX_PIN_UART2);
//   Serial.println("UART2 Ready for GPS");

//   // SoftwareSerialの初期化
//   mySerial.begin(9600);
//   Serial.println("SoftwareSerial Ready for ESP32 Communication");

//   // ランダムシードを設定
//   randomSeed(analogRead(0));
// }

// void loop() {
//   GPSData gpsData;

//   // GPSデータの受信
//   while (Serial2.available() > 0) {
//     char c = Serial2.read();
//     gps.encode(c); // TinyGPSにデータを渡す
//   }

//   // GPSデータが利用可能な場合は更新、利用できない場合はデフォルト値を使用
//   if (gps.location.isUpdated()) {
//     gpsData.latitude = gps.location.lat();
//     gpsData.longitude = gps.location.lng();
//   } else {
//     gpsData.latitude = 0.0; // デフォルト値
//     gpsData.longitude = 0.0; // デフォルト値
//   }

//   // ランダムな値を生成
//   gpsData.randomValue = random(0, 100);

//   // デバッグ用にGPSデータを表示
//   Serial.print("GPS Data - Lat: ");
//   Serial.print(gpsData.latitude, 6);
//   Serial.print(", Lon: ");
//   Serial.print(gpsData.longitude, 6);
//   Serial.print(", Random: ");
//   Serial.println(gpsData.randomValue);

//   // SoftwareSerialで構造体を送信
//   mySerial.write((uint8_t*)&gpsData, sizeof(gpsData));
//   Serial.println("GPS Data sent via SoftwareSerial");

//   delay(100); // 10Hzで送信
// }















// #include <Arduino.h>
// #include <SoftwareSerial.h>

// // SoftwareSerialのピン設定 (送信側ESP32からの受信用)
// #define TX_PIN_SOFT 25 // SoftwareSerial送信ピン（未使用）
// #define RX_PIN_SOFT 26 // SoftwareSerial受信ピン

// SoftwareSerial mySerial(RX_PIN_SOFT, TX_PIN_SOFT); // SoftwareSerialのRX, TX

// // GPSデータを格納する構造体
// struct GPSData {
//   float latitude;
//   float longitude;
//   int randomValue; // ランダムな値を保持する変数
// };

// void setup() {
//   // デバッグ用シリアル通信
//   Serial.begin(115200);
//   Serial.println("ESP32 GPS Receiver Ready");

//   // SoftwareSerialの初期化
//   mySerial.begin(9600);
//   Serial.println("SoftwareSerial Ready for ESP32 Communication");
// }

// void loop() {
//   // SoftwareSerialで構造体を受信
//   if (mySerial.available() >= sizeof(GPSData)) {
//     GPSData receivedData;
//     mySerial.readBytes((uint8_t*)&receivedData, sizeof(receivedData));

//     // 受信したGPSデータを表示
//     Serial.print("Received GPS Data - Lat: ");
//     Serial.print(receivedData.latitude, 6);
//     Serial.print(", Lon: ");
//     Serial.print(receivedData.longitude, 6);
//     Serial.print(", Random: ");
//     Serial.println(receivedData.randomValue);
//   }

//   delay(100); // 少し待機
// }









// #include <Arduino.h>

// void setup() {
//   Serial.begin(115200); // シリアル通信の初期化
// }

// void loop() {
//   Serial.println("Hello, World!"); // メッセージをシリアルモニターに表示
//   Serial.println(millis());
//   delay(100); // 1秒待機
// }













// // このコードで現在地情報の取得に成功した
// #include <TinyGPSPlus.h>
// #include <SoftwareSerial.h>

// /*
//    This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
//    It requires the use of SoftwareSerial, and assumes that you have a
//    4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
// */
// static const int RXPin = 17, TXPin = 16;
// static const uint32_t GPSBaud = 9600;

// // The TinyGPSPlus object
// TinyGPSPlus gps;
// void displayInfo();

// // The serial connection to the GPS device
// SoftwareSerial ss(RXPin, TXPin);

// // タイマーの設定
// unsigned long previousMillis = 0;
// const long interval = 1000; // 1秒ごとに表示

// void setup()
// {
//   Serial.begin(115200);
//   ss.begin(GPSBaud);

//   Serial.println(F("DeviceExample.ino"));
//   Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
//   Serial.print(F("Testing TinyGPSPlus library v. "));
//   Serial.println(TinyGPSPlus::libraryVersion());
//   Serial.println(F("by Mikal Hart"));
//   Serial.println();
// }

// void loop()
// {
//   // This sketch displays information every time a new sentence is correctly encoded.
//   while (ss.available() > 0)
//     if (gps.encode(ss.read()))
//       ; // データのエンコードのみ行う

//   unsigned long currentMillis = millis();
//   if (currentMillis - previousMillis >= interval)
//   {
//     previousMillis = currentMillis;
//     displayInfo();
//   }

//   if (millis() > 1000 && gps.charsProcessed() < 10)
//   {
//     Serial.println(F("No GPS detected: check wiring."));
//     while (true)
//       ;
//   }
// }

// void displayInfo()
// {
//   Serial.print(F("Location: "));
//   if (gps.location.isValid())
//   {
//     Serial.print(gps.location.lat(), 6);
//     Serial.print(F(","));
//     Serial.print(gps.location.lng(), 6);
//   }
//   else
//   {
//     Serial.print(F("INVALID"));
//   }

//   Serial.print(F("  Date/Time: "));
//   if (gps.date.isValid())
//   {
//     Serial.print(gps.date.month());
//     Serial.print(F("/"));
//     Serial.print(gps.date.day());
//     Serial.print(F("/"));
//     Serial.print(gps.date.year());
//   }
//   else
//   {
//     Serial.print(F("INVALID"));
//   }

//   Serial.print(F(" "));
//   if (gps.time.isValid())
//   {
//     if (gps.time.hour() < 10)
//       Serial.print(F("0"));
//     Serial.print(gps.time.hour());
//     Serial.print(F(":"));
//     if (gps.time.minute() < 10)
//       Serial.print(F("0"));
//     Serial.print(gps.time.minute());
//     Serial.print(F(":"));
//     if (gps.time.second() < 10)
//       Serial.print(F("0"));
//     Serial.print(gps.time.second());
//     Serial.print(F("."));
//     if (gps.time.centisecond() < 10)
//       Serial.print(F("0"));
//     Serial.print(gps.time.centisecond());
//   }
//   else
//   {
//     Serial.print(F("INVALID"));
//   }

//   Serial.println();
// }






// #include <Arduino.h>
// const int PIN_SER   =  7;
// const int PIN_LATCH =  5;
// const int PIN_CLK   = 6;

// byte patterns[] = {
//   B00000000,
//   B10000000,
//   B11000000,
//   B11100000,
//   B11110000,
//   B11111000,
//   B01111100,
//   B00111110,
//   B00011111,
//   B00001111,
//   B00000111,
//   B00000011,
//   B00000001,
//   B00000000
// };

// void setup() {
//   pinMode( PIN_SER, OUTPUT );
//   pinMode( PIN_LATCH, OUTPUT );
//   pinMode( PIN_CLK, OUTPUT );
  
//   byte b = B11000000;
  
//   digitalWrite(PIN_LATCH, LOW);
//   shiftOut(PIN_SER, PIN_CLK, LSBFIRST, patterns[b]);
//   digitalWrite(PIN_LATCH, HIGH);

//   delay(5000);
// }

// void loop() {
//   for ( int i=0; i <sizeof(patterns)/sizeof(byte); i++ ) {
//     digitalWrite( PIN_LATCH, LOW );
//     shiftOut( PIN_SER, PIN_CLK, LSBFIRST, patterns[i] );
//     digitalWrite( PIN_LATCH, HIGH );
//     delay(200);  
//   }
// }









// #include <Arduino.h>
// #include "FS.h"
// #include "SD.h"
// #include "SPI.h"

// int sck = 18;
// int miso = 19;
// int mosi = 23;
// int cs = 5;

// unsigned long previousMillis = 0; // 前回の時間を記憶
// const long interval = 100; // インターバル時間 (1秒)

// void setup() {
//   Serial.begin(115200);

//   SPI.begin(sck, miso, mosi, cs);
//   if (!SD.begin(cs)) {
//     Serial.println("Card Mount Failed");
//     return;
//   }
//   Serial.println("SD Card initialized");
// }

// void loop() {
//   unsigned long currentMillis = millis();
//   if (currentMillis - previousMillis >= interval) {
//     previousMillis = currentMillis;
    
//     File file = SD.open("/data.txt", FILE_APPEND);
//     if (file) {
//       file.println("Hello World 表面実装");
//       file.close();
//       Serial.println("Data written to file");
//     } else {
//       Serial.println("Failed to open file for writing");
//     }
//   }
// }
