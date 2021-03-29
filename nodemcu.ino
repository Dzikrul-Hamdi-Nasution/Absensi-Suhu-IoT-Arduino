#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h> 
#include <ESP8266HTTPClient.h>



#define FIREBASE_HOST "suhu-71239.firebaseio.com"
#define FIREBASE_AUTH "LZwXV342Hvqhh0td63hcwlKv58mJnAph8CohwTIM"
#define WIFI_SSID "Cicinn"
#define WIFI_PASSWORD "cicincicin"

//#define WIFI_SSID "Server Beta V2"
//#define WIFI_PASSWORD "Kosong12345"

//#define WIFI_SSID "SALAM ELECTRONICS"
//#define WIFI_PASSWORD "btn-primary"

String hari[5] = {"senin", "selasa", "rabu", "kamis", "jumat"};
String nama[4] = {};
String Status[4] = {};
String suhu[4] = {};
void setup() {

  Serial.begin(9600);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //checkSlot();

}

void loop() {
  check_reset();
  receiveData();

}

void check_reset(){
    if (Firebase.getString("reset/nilai") == "1") {
    Firebase.setString("reset/nilai", "0");
    Firebase.setString("hari/nilai", "senin_mk_1");
    Firebase.setString("nama1_senin_mk_1/nilai", "empty");
    Firebase.setString("nama2_senin_mk_1/nilai", "empty");
    Firebase.setString("nama3_senin_mk_1/nilai", "empty");

    Firebase.setString("nama1_senin_mk_2/nilai", "empty");
    Firebase.setString("nama2_senin_mk_2/nilai", "empty");
    Firebase.setString("nama3_senin_mk_2/nilai", "empty");

    Firebase.setString("nama1_selasa_mk_1/nilai", "empty");
    Firebase.setString("nama2_selasa_mk_1/nilai", "empty");
    Firebase.setString("nama3_selasa_mk_1/nilai", "empty");

    Firebase.setString("nama1_selasa_mk_2/nilai", "empty");
    Firebase.setString("nama2_selasa_mk_2/nilai", "empty");
    Firebase.setString("nama3_selasa_mk_2/nilai", "empty");

    Firebase.setString("nama1_rabu_mk_1/nilai", "empty");
    Firebase.setString("nama2_rabu_mk_1/nilai", "empty");
    Firebase.setString("nama3_rabu_mk_1/nilai", "empty");

    Firebase.setString("nama1_rabu_mk_2/nilai", "empty");
    Firebase.setString("nama2_rabu_mk_2/nilai", "empty");
    Firebase.setString("nama3_rabu_mk_2/nilai", "empty");

    Firebase.setString("nama1_kamis_mk_1/nilai", "empty");
    Firebase.setString("nama2_kamis_mk_1/nilai", "empty");
    Firebase.setString("nama3_kamis_mk_1/nilai", "empty");

    Firebase.setString("nama1_kamis_mk_2/nilai", "empty");
    Firebase.setString("nama2_kamis_mk_2/nilai", "empty");
    Firebase.setString("nama3_kamis_mk_2/nilai", "empty");

    Firebase.setString("nama1_jumat_mk_1/nilai", "empty");
    Firebase.setString("nama2_jumat_mk_1/nilai", "empty");
    Firebase.setString("nama3_jumat_mk_1/nilai", "empty");

    Firebase.setString("nama1_jumat_mk_2/nilai", "empty");
    Firebase.setString("nama2_jumat_mk_2/nilai", "empty");
    Firebase.setString("nama3_jumat_mk_2/nilai", "empty");
  }
}

byte rec = 1, cek = 0;
void receiveData() {
  if (Serial.available() > 0) {
    char In = Serial.read();
    Serial.print(In);
    while (In == '#') {
      if (Serial.available() > 0) {
        char c = Serial.read();

        if (c != '@') {
          if (c != '|') {
            if (rec == 1) {
              nama[cek] += c;
            }
            else if (rec == 2) {
              Status[cek] += c;
            }
            else if (rec == 3) {
              suhu[cek] += c;
            }
          }
          else {
            rec ++;
            if (rec == 4) {
              rec = 1;
              cek ++;
            }
          }

        }

        else {
          checkSlot();
            
          for (int z = 0; z < 3; z++) {
           // Serial.print(nama[z]);
            //Serial.print(Status[z]);
            //Serial.println(suhu[z]);

            nama[z] = "";
            Status[z] = "";
            suhu[z] = "";
            In = '0';
            rec = 1, cek = 0;
          }

        }
      }
    }
    while (In == '*') {
      if (Serial.available() > 0) {
        char c = Serial.read();
        if (c != '@') {
          if (c != '|') {
            if (rec == 1) {
              nama[3] += c;
            }
            else if (rec == 2) {
              Status[3] += c;
            }
            else if (rec == 3) {
              suhu[3] += c;
            }
          }
          else {
            rec ++;
          }
        }
        else {
          rec = 1;
          In = '0';
        //  Serial.println(nama[3]);
        //  Serial.println(suhu[3]);
         // Serial.println(Status[3]);
          if (nama[3] == "AFIFAH") {
            Firebase.setString("nama1/nilai", nama[3] + " " + suhu[3]);
            Firebase.setString("status1/nilai", Status[3]);

          }
          else if (nama[3] == "CHINDY") {
            Firebase.setString("nama2/nilai", nama[3] + " " + suhu[3]);
            Firebase.setString("status2/nilai", Status[3]);

          }
          else if (nama[3] == "AKSA") {
            Firebase.setString("nama3/nilai", nama[3] + " " + suhu[3]);
            Firebase.setString("status3/nilai", Status[3]);

          }
          nama[3]="";
          suhu[3]="";
          Status[3]="";

        }
      }
    }
  }
}

void checkSlot() {
  if (Firebase.getString("nama1_senin_mk_1/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "senin_mk_1");
    Firebase.setString("nama1_senin_mk_1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_senin_mk_1/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_senin_mk_1/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_senin_mk_1/nilai", Status[0]);
    Firebase.setString("status2_senin_mk_1/nilai", Status[1]);
    Firebase.setString("status3_senin_mk_1/nilai", Status[2]);
  }
  else if (Firebase.getString("nama1_senin_mk_2/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "senin_mk_2");
    Firebase.setString("nama1_senin_mk_2/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_senin_mk_2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_senin_mk_2/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_senin_mk_2/nilai", Status[0]);
    Firebase.setString("status2_senin_mk_2/nilai", Status[1]);
    Firebase.setString("status3_senin_mk_2/nilai", Status[2]);
  }

  else if (Firebase.getString("nama1_selasa_mk_1/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "selasa_mk_1");
    Firebase.setString("nama1_selasa_mk_1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_selasa_mk_1/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_selasa_mk_1/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_selasa_mk_1/nilai", Status[0]);
    Firebase.setString("status2_selasa_mk_1/nilai", Status[1]);
    Firebase.setString("status3_selasa_mk_1/nilai", Status[2]);
  }
  else if (Firebase.getString("nama1_selasa_mk_2/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "selasa_mk_2");
    Firebase.setString("nama1_selasa_mk_2/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_selasa_mk_2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_selasa_mk_2/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_selasa_mk_2/nilai", Status[0]);
    Firebase.setString("status2_selasa_mk_2/nilai", Status[1]);
    Firebase.setString("status3_selasa_mk_2/nilai", Status[2]);
  }

  else if (Firebase.getString("nama1_rabu_mk_1/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "rabu_mk_1");
    Firebase.setString("nama1_rabu_mk_1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_rabu_mk_1/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_rabu_mk_1/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_rabu_mk_1/nilai", Status[0]);
    Firebase.setString("status2_rabu_mk_1/nilai", Status[1]);
    Firebase.setString("status3_rabu_mk_1/nilai", Status[2]);
  }
  else if (Firebase.getString("nama1_rabu_mk_2/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "rabu_mk_2");
    Firebase.setString("nama1_rabu_mk_2/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_rabu_mk_2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_rabu_mk_2/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_rabu_mk_2/nilai", Status[0]);
    Firebase.setString("status2_rabu_mk_2/nilai", Status[1]);
    Firebase.setString("status3_rabu_mk_2/nilai", Status[2]);
  }

  else if (Firebase.getString("nama1_kamis_mk_1/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "kamis_mk_1");
    Firebase.setString("nama1_kamis_mk_1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_kamis_mk_1/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_kamis_mk_1/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_kamis_mk_1/nilai", Status[0]);
    Firebase.setString("status2_kamis_mk_1/nilai", Status[1]);
    Firebase.setString("status3_kamis_mk_1/nilai", Status[2]);
  }
  else if (Firebase.getString("nama1_kamis_mk_2/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "kamis_mk_2");
    Firebase.setString("nama1_kamis_mk_2/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_kamis_mk_2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_kamis_mk_2/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_kamis_mk_2/nilai", Status[0]);
    Firebase.setString("status2_kamis_mk_2/nilai", Status[1]);
    Firebase.setString("status3_kamis_mk_2/nilai", Status[2]);
  }

  else if (Firebase.getString("nama1_jumat_mk_1/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "jumat_mk_1");
    Firebase.setString("nama1_jumat_mk_1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_jumat_mk_1/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_jumat_mk_1/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_jumat_mk_1/nilai", Status[0]);
    Firebase.setString("status2_jumat_mk_1/nilai", Status[1]);
    Firebase.setString("status3_jumat_mk_1/nilai", Status[2]);
  }
  else if (Firebase.getString("nama1_jumat_mk_2/nilai") == "empty") {
    Firebase.setString("nama1/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1/nilai", Status[0]);
    Firebase.setString("status2/nilai", Status[1]);
    Firebase.setString("status3/nilai", Status[2]);
    Firebase.setString("hari/nilai", "jumat_mk_2");
    Firebase.setString("nama1_jumat_mk_2/nilai", nama[0] + " " + suhu[0]);
    Firebase.setString("nama2_jumat_mk_2/nilai", nama[1] + " " + suhu[1]);
    Firebase.setString("nama3_jumat_mk_2/nilai", nama[2] + " " + suhu[2]);
    Firebase.setString("status1_jumat_mk_2/nilai", Status[0]);
    Firebase.setString("status2_jumat_mk_2/nilai", Status[1]);
    Firebase.setString("status3_jumat_mk_2/nilai", Status[2]);
    Firebase.setString("reset/nilai", "1");

  }

}
