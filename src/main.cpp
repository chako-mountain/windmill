
// [env:esp32]
// platform = espressif32
// board = esp32dev
// framework = arduino
// lib_deps = 
//     ; regenbogencode/ESPNowW@^1.0.2
//     ; mikalhart/TinyGPSPlus@^1.1.0
//     ; adafruit/Adafruit BNO055@^1.6.4
// monitor_speed = 115200

// ; [env:uno]
// ; platform = atmelavr
// ; board = uno
// ; framework = arduino
// ; lib_deps = 
// ;     ; mikalhart/TinyGPSPlus@^1.1.0
// ;     ; adafruit/Adafruit BNO055@^1.6.4
// ; monitor_speed = 9600










// 尾翼側で構造体を送信するコード
#include <Arduino.h>
#include "SoftwareSerial.h"

struct dakakuData {
  float rudder = 0;
  float elevetor = 0;
};

dakakuData data;
SoftwareSerial mySerial(12, 11);

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
}

void loop() {
  data.rudder = random(0, 100);  // ランダムな値
  data.elevetor = random(0, 100);

  Serial.print(data.rudder);  // rudderの値を表示

  mySerial.write((uint8_t*)&data, sizeof(data));
  Serial.println("Data sent!");
  delay(100); // 適切な送信間隔を設定
}













// //mainOBC側で構造体を受信するコード
// //また、ESP32からのリクエストに応じて構造体データを送信
// #include <Arduino.h>
// #include "SoftwareSerial.h"
// #include <Wire.h>

// const int DATA = 8;
// const int LATCH = 10;
// const int CLOCK = 9;

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
//   Serial.println(data.rudder);
//   Serial.print("elevetor: ");
//   Serial.println(data.elevetor);
//   Serial.print("hight: ");
//   Serial.println(data2.hight);

//   delay(100); // 適切な受信間隔を設定

// }




















// // ESP32側で、とりあえずUnoからのデータを受信
// //姿勢、RPMを追加
// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BNO055.h>
// #include <utility/imumaths.h>
// #include <TinyGPSPlus.h>
// #include "FS.h"
// #include "SD.h"
// #include "SPI.h"

// int sck = 18;
// int miso = 19;
// int mosi = 23;
// int cs = 5;

// struct withHight {
//   float rudder = 0;
//   float elevetor = 0;
//   float hight = 0;
// };

// struct mainData {
//   float rudder = 0;
//   float elevetor = 0;
//   float hight = 0;
//   float RPM = 0;
//   float roll = 0;
//   float pitch = 0;
//   float yaw = 0;
//   float hour = 0;
//   float seconds = 0;
//   float latitude = 0;
//   float longitude = 0;
// };

// #define SLAVE_ADDRESS 0x08 // スレーブのアドレス

// #define SLAVE_ADDRESS2 0x04

// Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);
// withHight datawithhight; // 受信データ格納用構造体
// mainData maindata;


// #define ENCODER_PIN_A 14
// #define ENCODER_PIN_B 15

// volatile long pulseCount = 0;
// unsigned long lastMillis = 0;

// #define RXD2 17
// #define TXD2 16
// #define GPS_BAUD 9600

// TinyGPSPlus gps;
// HardwareSerial gpsSerial(2);



// void countPulse() {
//   if (digitalRead(ENCODER_PIN_B) == HIGH) {
//     pulseCount++;
//   } else {
//     pulseCount--;
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   Wire.begin(); // マスターとして初期化
//   gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);


//   pinMode(ENCODER_PIN_A, INPUT);
//   pinMode(ENCODER_PIN_B, INPUT);
//   attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), countPulse, RISING);

//   pinMode(4,OUTPUT);

  
  

//   if (!bno.begin()) {
//     /* There was a problem detecting the BNO055 ... check your connections */
//     Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
//     while (1)
//       ;
//   }

//   SPI.begin(sck, miso, mosi, cs);
//   if (!SD.begin(cs)) {
//     Serial.println("Card Mount Failed");
//     return;
//   }
//   Serial.println("SD Card initialized");
// }

// void loop() {
  
//   Wire.requestFrom(SLAVE_ADDRESS, sizeof(datawithhight)); // 構造体サイズ分データ要求

//   if (Wire.available() >= sizeof(datawithhight)) { // 必要なバイト数があるか確認
//     Wire.readBytes((uint8_t*)&datawithhight, sizeof(datawithhight)); // 構造体全体を取得
    
//     // 受信データを表示
//     // Serial.print("Received rudder: ");
//     // Serial.println(datawithhight.rudder);
//     // Serial.print("Received elevetor: ");
//     // Serial.println(datawithhight.elevetor);
//     // Serial.print("Received hight: ");
//     // Serial.println(datawithhight.hight);

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

//   if(gps.location.isUpdated()) {
//     maindata.latitude = gps.location.lat();
//     maindata.longitude = gps.location.lng();
//   }
//   if(gps.time.isUpdated()) {
//     maindata.hour = gps.time.hour();
//     maindata.seconds = gps.time.second();
//   }




//   Serial.print("Received rudder: ");
//   Serial.println(datawithhight.rudder);
//   Serial.print("Received elevetor: ");
//   Serial.println(datawithhight.elevetor);
//   Serial.print("Received hight: ");
//   Serial.println(datawithhight.hight);
//   Serial.print("Received roll: ");
//   Serial.println(maindata.roll);
//   Serial.print("Received pitch: ");
//   Serial.println(maindata.pitch);
//   Serial.print("Received yaw: ");
//   Serial.println(maindata.yaw);
//   Serial.print("Received RPM: ");
//   Serial.println(maindata.RPM);
//   Serial.print("Received hour: ");
//   Serial.println(maindata.hour);
//   Serial.print("Received seconds: ");
//   Serial.println(maindata.seconds);
//   Serial.print("Received latitude: ");
//   Serial.println(maindata.latitude, 6);
//   Serial.print("Received longitude: ");
//   Serial.println(maindata.longitude, 6);

//   File file = SD.open("/data.txt", FILE_APPEND);  if (file) {
//     file.print("rudder: ");
//     file.println(maindata.rudder);
//     file.print("elevetor: ");
//     file.println(maindata.elevetor);
//     file.print("hight: ");
//     file.println(maindata.hight);
//     file.print("roll: ");
//     file.println(maindata.roll);
//     file.print("pitch: ");
//     file.println(maindata.pitch);
//     file.print("yaw: ");
//     file.println(maindata.yaw);
//     file.print("RPM: ");
//     file.println(maindata.RPM);
//     file.print("hour: ");
//     file.println(maindata.hour);
//     file.print("seconds: ");
//     file.println(maindata.seconds);
//     file.print("latitude: ");
//     file.println(maindata.latitude, 6);
//     file.print("longitude: ");
//     file.println(maindata.longitude, 6);
    
//     Serial.println("Data written to SD card");
    
//   } else {
//     Serial.println("Error opening data.txt");
//   }
//   file.close(); // ファイルを閉じる

//   Wire.beginTransmission(SLAVE_ADDRESS2); 
//   Wire.write((uint8_t*)&maindata, sizeof(maindata)); // 構造体を送信
//   Wire.endTransmission();
//   Serial.println("Data sent to slave!");


  
  
//   delay(100); // 送信頻度調整
//   digitalWrite(4, LOW);
// }












//  メイン基板から送信されてきたデータをI2Cで受信する
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














//　I2Cで受信したデータをfirebaseに送信するコード
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
//     if (currentMillis - lastSendTime >= 1000) { // 1秒ごとに送信
//         lastSendTime = currentMillis;
//         sendToFirebase();
//     }
// }














// 以下は特に関係のないコード






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