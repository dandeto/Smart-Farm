 /*
  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
*/

#include "heltec.h"
#include "images.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_seesaw.h"

#define BAND 915E6  //you can set band here directly,e.g. 868E6,915E6
#define SEALEVELPRESSURE_HPA (1013.25)

unsigned int counter = 0;
String rssi = "RSSI --";
String packet;
int id = 0;
long token = 0;
volatile bool rx = false;
Adafruit_BME280 bme;
Adafruit_seesaw ss;
float temperature, pressure, altitude, humidity; 
uint16_t capactive;
String type; // works great when not volatile
volatile int pdata;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

// This ISR will run even during a delay function
void getPacket(int size) {
  if(size) rx = true;
  type = "";
  type += (char)LoRa.read(); // get first 2 bytes. check if this is an ID assignment.
  type += (char)LoRa.read();
  pdata = LoRa.parseInt();
  //Serial.println("Packet: " + type + String(pdata)); // debug
}

void setup()
{
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();

  // Setup Soil Sensor
  Serial.println("Init Serial");

  // Initialize BME280
  while (!bme.begin()) {
    Heltec.display->drawString(0, 0, "ERROR! BME not found");
    Heltec.display->display();
    delay(100);
  }
  
  Heltec.display->clear();
  
  // Initialize Seesaw
  while (!ss.begin(0x36)) { // i2c address for capacitance sensor is 0x36
    Heltec.display->drawString(0, 0, "ERROR! Seesaw not found");
    Heltec.display->display();
    delay(100);
  }

  /*
   * LoRa.setTxPower(txPower,RFOUT_pin);
   * txPower -- 0 ~ 20
   * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
   *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
   *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
  */
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  
  // wait for prompt to send data
  LoRa.onReceive(getPacket); // callback for when receive done
  //LoRa.onTxDone(onTxDone); // callback for when transmission done
  LoRa.receive(); // put radio in receive mode
}

void loop() {
  while(!id) {
    Heltec.display->clear();
    Heltec.display->drawString(0, 0,  id ? "ID: " + String(id) : "Waiting for ID...");
    Heltec.display->display();
    // ask for id
    LoRa.beginPacket();
    LoRa.print("ID ");
    token = random(1,247483647);
    LoRa.print(token);
    LoRa.endPacket();
    LoRa.receive();
    // wait 5 seconds
    Serial.println("Ask for ID");
    int count = 0;
    while(type != "ID" && ++count < 2000) { // wait 2 seconds
      delay(10);
    }
    if(type == "ID") {
      Serial.println("NO ID, pkt type: " + String(type));
      id = pdata;
    }
  }
  
  // respond to request for data
  if(rx) {
    Serial.println("Packet: " + type + String(pdata));
      if(type == "RQ" && pdata == id) {
        do {
          LoRa.beginPacket();
          LoRa.print(id);
          LoRa.print(",");
          LoRa.print(temperature);
          LoRa.print(",");
          LoRa.print(pressure);
          LoRa.print(",");
          LoRa.print(altitude);
          LoRa.print(",");
          LoRa.print(humidity);
          LoRa.print(",");
          LoRa.print(capactive);
        } while(!LoRa.endPacket()); // returns 1 on success 0 on failure
        LoRa.receive(); // yes, actually need to put it back into receive mode after sending
        
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        Serial.println("RESPOND");
      } else if(type == "RS" && pdata == 0) { // server sends 0 means reset
        Serial.println("RESET");
        id = 0;
        do {
          LoRa.beginPacket();
          LoRa.print("ID");
        } while(!LoRa.endPacket());
        LoRa.receive();
      }
    
    rx = false;
  }

  // get sensor data
  temperature = bme.readTemperature();
  pressure    = bme.readPressure() / 100.0F;
  altitude    = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humidity    = bme.readHumidity();
  capactive   = ss.touchRead(0);

  // print sensor data
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0,  id ? "ID: " + String(id) : "Waiting for ID...");
  Heltec.display->drawString(0, 10, "Temperature: " + String(temperature) + " °C");
  Heltec.display->drawString(0, 20, "Capacitive: " + String(capactive));
  Heltec.display->drawString(0, 30, "Pressure: " + String(pressure) + " hPa");
  Heltec.display->drawString(0, 40, "Approx. Altitude: " + String(altitude) + " m");
  Heltec.display->drawString(0, 50, "Humidity: " + String(humidity) + " %");
  Heltec.display->display();

  delay(2000); // wait for 2 seconds
}
