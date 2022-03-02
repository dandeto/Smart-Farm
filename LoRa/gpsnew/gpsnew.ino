#include "TinyGPS++.h"
#include "HardwareSerial.h"
#include "heltec.h"

#define BAND 915E6  //you can set band here directly,e.g. 868E6,915E6

float latitude , longitude;
String  lat_str , lng_str;
TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

void setup()
{
    Serial.begin(115200);
    SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
}

void loop()
{
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read()))
    {
//      Serial.print("hello ");

//      if (gps.location.isValid())
//      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        Serial.print("Latitude = ");
        Serial.println(lat_str);
        Serial.print("Longitude = ");
        Serial.println(lng_str);
//      }
     delay(1000);
     Serial.println();  
    }
  }  
}
