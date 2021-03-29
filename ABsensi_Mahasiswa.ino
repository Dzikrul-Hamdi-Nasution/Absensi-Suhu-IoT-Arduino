#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#define SS_PIN 10
#define RST_PIN 9
#define alarm 8

String ID[5] = {"AAC09116", "EB26A1C", "BC9ED54", "B152E312", "61824313"};
String Name[5] = {"AFIFAH", "CHINDY", "AKSA", "DOSEN", "DOSEN"};
String Status[5] = {"alpha", "alpha", "alpha", "alpha", "alpha"};
float temp[5] = {};

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
SoftwareSerial kirim(16, 17); // RX | TX
MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[3];

void setup() {

  pinMode(alarm, OUTPUT);
  digitalWrite(alarm, HIGH);
  Serial.begin(9600);
  kirim.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Pengecekan Suhu");
  lcd.setCursor(0, 1);
  lcd.print("dan Absensi");
  delay(10000);
  mlx.begin();
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  begin_();
}

void begin_() {
  lcd.clear();
  //Serial.println("Silahkan Absen Sebelum Masuk");
  //Serial.println("Tempelkan IDchar Mu");
  lcd.clear();
  lcd.print("Tempelkan Kartu");
}

void loop() {

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  byte dec = 0;
  for (byte i = 0; i < 5; i++) {
    if (ID[i] != printHex(rfid.uid.uidByte, rfid.uid.size)) {
      dec++;
      // Serial.print(dec);
    }
    if (dec == 5) {
      lcd.clear();
      lcd.print("ID Tdk Terdaftar");
      delay(2000);
      begin_();

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      return;
    }
  }

  lcd.clear();
  lcd.print("Scan Suhu");


  byte a = 1;
  unsigned long timer;
  timer = millis();
  //Serial.println("Silahkan Scan Suhu");

  while (a == 1) {
    alarmON();
    float suhu = mlx.readObjectTempC();
    float suhu_2 = suhu;
    //Serial.println(suhu);
    suhu = (0.4695459 * suhu) + 20.67419712;
    //suhu = suhu + 5;
    //Serial.print("Suhu tubuh = "); Serial.print(suhu); Serial.println("*C");
    lcd.setCursor(0, 1);
    lcd.print("Suhu : ");
    lcd.print(suhu);

    if (suhu >= 35 && suhu < 40) {

      if (millis() > timer + 3000) {
        a = 0;
        if (suhu > 37.4) {
          lcd.clear();
          lcd.print("Overheat");
          delay(3000);
          a = 0;
          begin_();
          timer = millis();
          digitalWrite(alarm, HIGH);
          return;
        }
        // Serial.println();
        // Serial.println();
        for (byte i = 0; i < 5; i++) {
          if (ID[i] == printHex(rfid.uid.uidByte, rfid.uid.size)) {
            if (Name[i] != "DOSEN") {
              lcd.clear();
              lcd.print("Nama :");
              lcd.print(Name[i]);
              lcd.setCursor(0, 1);
              lcd.print("Suhu :");
              lcd.print(suhu);
              lcd.write(B11011111);
              lcd.print("C");
              digitalWrite(alarm, HIGH);
              temp[i] = suhu;
              if (suhu >= 37.5) {
                Status[i] = "Sakit";
              }
              else {
                Status[i] = "Hadir";
              }
              //Serial.print(Name[i]);
              //Serial.println(Status[i]);
              //Serial.println(temp[i]);
              //digitalWrite(alarm, HIGH);
              kirim.print('*');
              delay(300);
              kirim.print(Name[i]);
              kirim.print('|');
              kirim.print(Status[i]);
              kirim.print('|');
              kirim.print(temp[i]);
              kirim.print('|');
              kirim.print('@');
              delay(15000);

            }
            else {
              // Serial.println("Dosen dah Masuk");
              lcd.clear();
              lcd.print("absen terkumpul");
              digitalWrite(alarm, HIGH);
              kirim.print('#');
              delay(300);
              for (int z = 0; z < 3; z++) {
                kirim.print(Name[z]);
                kirim.print('|');
                kirim.print(Status[z]);
                kirim.print('|');
                kirim.print(temp[z]);
                kirim.print('|');

                Status[z] = "alpha";
                temp[z] = 0;
              }
              kirim.print('@');

              delay(15000);
            }
          }
        }

        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        // Serial.println();
        begin_();
      }
    }
    else {
      timer = millis();
    }
  }
}


String printHex(byte * buffer_, byte bufferSize) {
  String val = "";
  for (byte i = 0; i < bufferSize; i++) {
    val += String(buffer_[i], HEX);

  }
  val.toUpperCase();
  //Serial.println(val);
  return val;
}

unsigned long intervalON;
void alarmON() {
  if (millis() > intervalON + 700 && millis() < intervalON + 850) {
    digitalWrite(alarm, HIGH);
  }
  if (millis() > intervalON + 850) {
    digitalWrite(alarm, LOW);
    intervalON = millis();
  }
}
