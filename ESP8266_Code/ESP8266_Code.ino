#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h> 

/*** User defined params for WiFI and FireBase App ***/
#define WIFI_SSID       "Makine Takip Sistemi"  // WiFi SSID
#define WIFI_PW         "password123"  // Wifi password
#define FIREBASE_APP    "makine-takip-sistemi.firebaseio.com"  // Firebase uygulama addresi, "<yourAppName>.firebaseio.com"
#define FIREBASE_SECRET "YrIc42z3NRkfO6C1Z5xvpasccWvelwjCN83uFCXe"  // FireBase tarafından sağlanan gizli kod, "ABcd1eF23gHIjKlMNOpqR456St7UVWXyZAB89cDE"

int timeold = 0;
char MAC_char[18];
int16_t tempNum, rpm, currentNum, vib;

void setup()
{
  Wire.begin(0,2);
  Serial.begin(115200);
  // connect to wifi.
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("Bağlanıyor");
  WiFi.begin(WIFI_SSID, WIFI_PW);
  int x = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
    if(x++>20)  // timeout
       break;
  }

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("OK");
    Serial.println(WiFi.localIP());

    uint8_t MAC_array[6];
    WiFi.macAddress(MAC_array);
    for (int i = 0; i < sizeof(MAC_array); ++i){
      sprintf(MAC_char,"%s%02x:",MAC_char,MAC_array[i]);
    }
    Serial.println("MAC ");
    Serial.println(MAC_char);
  }
  else
    Serial.println("Zaman Aşımı");

  Firebase.begin(FIREBASE_APP, FIREBASE_SECRET);
  delay(3000);

  Serial.println("Başlatıldı.");

}

void loop()
{ 
  byte a[8];
  Wire.requestFrom(54,8);

  for(int i=0; i<8; ++i){
  a[i]=Wire.read();
}

 tempNum = a[0];
 tempNum = tempNum << 8 | a[1];
 rpm = a[2];
 rpm = rpm << 8 | a[3];
 currentNum = a[4];
 currentNum = currentNum << 8 | a[5];
 vib = a[6];
 vib = vib << 8 | a[7];


  double temp = tempNum/100.0;
  double current = currentNum/100.0;
  
  Serial.println(temp);
  Serial.println(rpm);
  Serial.println(current);
  Serial.println(vib);
  char status;
          
          if (WiFi.status() ==  WL_CONNECTED)
          {
            StaticJsonBuffer<200> jsonBuffer;
            JsonObject& data = jsonBuffer.createObject();
            data["temp"] = String(temp,3);
            data["rpm"]  = String(rpm);
            data["current"]  = String(current,3);
            data["vib"]  = String(vib);

            Firebase.set("/Makine Takip Sistemi", JsonVariant(data));
          }
  }




