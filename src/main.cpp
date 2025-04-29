










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













// ESP32側で、とりあえずUnoからのデータを受信
//姿勢、RPMを追加
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <TinyGPSPlus.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

int sck = 18;
int miso = 19;
int mosi = 23;
int cs = 5;

struct withHight {
  float rudder = 0;
  float elevetor = 0;
  float hight = 0;
};

struct mainData {
  float rudder = 0;
  float elevetor = 0;
  float hight = 0;
  float RPM = 0;
  float roll = 0;
  float pitch = 0;
  float yaw = 0;
  float hour = 0;
  float seconds = 0;
  float latitude = 0;
  float longitude = 0;
};

#define SLAVE_ADDRESS 0x08 // スレーブのアドレス

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);
withHight datawithhight; // 受信データ格納用構造体
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



void countPulse() {
  if (digitalRead(ENCODER_PIN_B) == HIGH) {
    pulseCount++;
  } else {
    pulseCount--;
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(); // マスターとして初期化
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);


  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), countPulse, RISING);


  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }

  SPI.begin(sck, miso, mosi, cs);
  if (!SD.begin(cs)) {
    Serial.println("Card Mount Failed");
    return;
  }
  Serial.println("SD Card initialized");
}

void loop() {
  Wire.requestFrom(SLAVE_ADDRESS, sizeof(datawithhight)); // 構造体サイズ分データ要求

  if (Wire.available() >= sizeof(datawithhight)) { // 必要なバイト数があるか確認
    Wire.readBytes((uint8_t*)&datawithhight, sizeof(datawithhight)); // 構造体全体を取得
    
    // 受信データを表示
    // Serial.print("Received rudder: ");
    // Serial.println(datawithhight.rudder);
    // Serial.print("Received elevetor: ");
    // Serial.println(datawithhight.elevetor);
    // Serial.print("Received hight: ");
    // Serial.println(datawithhight.hight);

    maindata.rudder = datawithhight.rudder;
    maindata.elevetor = datawithhight.elevetor;
    maindata.hight = datawithhight.hight;
  }

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  maindata.yaw = euler.x();
  maindata.pitch = euler.y();
  maindata.roll = euler.z();
  maindata.RPM = pulseCount; 

  pulseCount = 0;
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if(gps.location.isUpdated()) {
    maindata.latitude = gps.location.lat();
    maindata.longitude = gps.location.lng();
  }
  if(gps.time.isUpdated()) {
    maindata.hour = gps.time.hour();
    maindata.seconds = gps.time.second();
  }




  Serial.print("Received rudder: ");
  Serial.println(datawithhight.rudder);
  Serial.print("Received elevetor: ");
  Serial.println(datawithhight.elevetor);
  Serial.print("Received hight: ");
  Serial.println(datawithhight.hight);
  Serial.print("Received roll: ");
  Serial.println(maindata.roll);
  Serial.print("Received pitch: ");
  Serial.println(maindata.pitch);
  Serial.print("Received yaw: ");
  Serial.println(maindata.yaw);
  Serial.print("Received RPM: ");
  Serial.println(maindata.RPM);
  Serial.print("Received hour: ");
  Serial.println(maindata.hour);
  Serial.print("Received seconds: ");
  Serial.println(maindata.seconds);
  Serial.print("Received latitude: ");
  Serial.println(maindata.latitude, 6);
  Serial.print("Received longitude: ");
  Serial.println(maindata.longitude, 6);

  File file = SD.open("/data.txt", FILE_APPEND);  if (file) {
    file.print("rudder: ");
    file.println(maindata.rudder);
    file.print("elevetor: ");
    file.println(maindata.elevetor);
    file.print("hight: ");
    file.println(maindata.hight);
    file.print("roll: ");
    file.println(maindata.roll);
    file.print("pitch: ");
    file.println(maindata.pitch);
    file.print("yaw: ");
    file.println(maindata.yaw);
    file.print("RPM: ");
    file.println(maindata.RPM);
    file.print("hour: ");
    file.println(maindata.hour);
    file.print("seconds: ");
    file.println(maindata.seconds);
    file.print("latitude: ");
    file.println(maindata.latitude, 6);
    file.print("longitude: ");
    file.println(maindata.longitude, 6);
    
    Serial.println("Data written to SD card");
    
  } else {
    Serial.println("Error opening data.txt");
  }
  file.close(); // ファイルを閉じる

  
  
  delay(100); // 送信頻度調整
}







