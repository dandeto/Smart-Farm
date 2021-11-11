#include "heltec.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_seesaw.h"

Adafruit_BME280 bme;
Adafruit_seesaw ss;

#define SEALEVELPRESSURE_HPA (1013.25)

// SCL: PIN 15
// SDA: PIN 4

#define BAND 915E6 // not really used for this test code

void setup() {
  //Serial.begin(115200);
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.println("Init Serial");

  // Initialize BME280
  while (!bme.begin()) {
    Serial.println("ERROR! BME not found");
    delay(100);
  }
  
  while (!ss.begin(0x36)) {
    Serial.println("ERROR! Seesaw not found");
    delay(100);
  }
  
  Serial.print("Seesaw started! Version: ");
  Serial.println(ss.getVersion(), HEX);
}

void loop() {
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

  // Seesaw
  Serial.print("Temperature: ");Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);

  // BME
  Serial.println("Temperature: " + String(bme.readTemperature()) + " °C");
  Serial.println("Pressure: " + String(bme.readPressure() / 100.0F) + " hPa");
  Serial.println("Approx. Altitude: " + String(bme.readAltitude(SEALEVELPRESSURE_HPA)) + " m");
  Serial.println("Humidity: " + String(bme.readHumidity()) + " %");
  Serial.println();
  
  delay(100);
}
