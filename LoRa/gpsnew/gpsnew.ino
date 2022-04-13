#include "TinyGPS++.h"
#include "HardwareSerial.h"
#include "heltec.h"

#define BAND 915E6  //you can set band here directly,e.g. 868E6,915E6

float latitude , longitude;
String  lat_str , lng_str, date_str , time_str;
int month, year;
TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

#define RXD2 16
#define TXD2 17

void setup()
{
    Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
    SerialGPS.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop()
{
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read()))
    {
        if (gps.location.isValid())
        {
          latitude = gps.location.lat();
          lat_str = String(latitude , 6);
          longitude = gps.location.lng();
          lng_str = String(longitude , 6);
          Serial.print("Latitude = ");
          Serial.println(lat_str);
          Serial.print("Longitude = ");
          Serial.println(lng_str);
        }
        if(gps.date.isValid())
        {
          date_str = gps.date.day();
          month = gps.date.month();
          year = gps.date.year();
          Serial.print("Date");
          Serial.println(date_str);
          Serial.print("Month");
          Serial.println(month);
          Serial.print("Year");
          Serial.println(year);
        }
    delay(1000);
    Serial.println();  
    }
  }  
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "Latitude: " + lat_str);
  Heltec.display->drawString(0, 10, "Longitude: " + lng_str);
  Heltec.display->drawString(0, 20, "Date: " + date_str);
  Heltec.display->drawString(0, 30, "Month: " + String(month));
  Heltec.display->drawString(0, 40, "Year: " + String(year));
  Heltec.display->display();
}
