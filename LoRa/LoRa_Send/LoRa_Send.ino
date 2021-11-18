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
String packSize = "--";
String packet;
int id = 0;
volatile bool rx = false;
int miss = 0;
Adafruit_BME280 bme;
Adafruit_seesaw ss;
float temperature, pressure, altitude, humidity; 
uint16_t capactive;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

// This ISR will run even during a delay function
void getPacket(int size) {
  if(size) rx = true;
}

void getId() {
  while(!id) {
    // send packet to ask for ID
    LoRa.beginPacket();
    LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
    LoRa.print("ID");
    LoRa.endPacket();

    // wait ~10s for ID
    int d = 0;
    while(++d < 1000) {
      Heltec.display->clear();
      Heltec.display->drawString(0, 0, "Waiting for id ");
      Heltec.display->display();
      if (LoRa.parsePacket()) {
        String type;
        type += (char)LoRa.read(); // get first 2 bytes. check if this is an ID assignment.
        type += (char)LoRa.read();
        if(type == "ID") {
          id = LoRa.parseInt();
          break;
        }
      }
      delay(10);
    }
  }
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

  // Initialize Seesaw
  while (!ss.begin(0x36)) { // i2c address for capacitance sensor is 0x36
    Heltec.display->drawString(0, 0, "ERROR! Seesaw not found");
    Heltec.display->display();
    delay(100);
  }
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Seesaw started! V");
  Heltec.display->drawString(85, 0, String(ss.getVersion(), HEX));
  Heltec.display->display();
  delay(2000);

  // get id
  getId();

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, " ID " + String(id));
  Heltec.display->display();
  // wait for prompt to send data
  LoRa.onReceive(getPacket);
  LoRa.receive(); // put radio in receive mode
}

void loop() {
  // respond to request for data
  if(rx) {
    if(!id) { // no id, ignore all packets except for one to set the id.
      String type;
      type += (char)LoRa.read(); // get first 2 bytes. check if this is an ID assignment.
      type += (char)LoRa.read();
      Serial.println("NO ID, pkt type: " + String(type));
      if(type == "ID") {
        id = LoRa.parseInt();
        miss = 0;
      } else if( ++miss > 10){
      // miss 10 times? Ask for ID again.
        miss = 0;
        // ask again
        LoRa.beginPacket();
        LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
        LoRa.print("ID");
        LoRa.endPacket();
        LoRa.receive();
      }
    } else {
      String type;
      type += (char)LoRa.read(); // get first 2 bytes. check if this is an ID assignment.
      type += (char)LoRa.read();
      Serial.println("pkt type: " + String(type));
      int pdata = LoRa.parseInt();
      Serial.println("PDATA: " + String(pdata));
      if(type == "RQ" && pdata == id) {
        LoRa.beginPacket();
        /*
         * LoRa.setTxPower(txPower,RFOUT_pin);
         * txPower -- 0 ~ 20
         * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
         *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
         *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
        */
        LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
        LoRa.print(temperature);
        LoRa.print(",");
        LoRa.print(pressure);
        LoRa.print(",");
        LoRa.print(altitude);
        LoRa.print(",");
        LoRa.print(humidity);
        LoRa.print(",");
        LoRa.print(capactive);  
        LoRa.endPacket();
        LoRa.receive(); // stay in receiver mode
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
        Serial.println("RESPOND");
      } else if(type == "RS" && pdata == 0) { // server sends 0 means reset
        Serial.println("RESET");
        id = 0;
        LoRa.beginPacket();
        LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
        LoRa.print("ID");
        LoRa.endPacket();
        LoRa.receive();
      }
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
  Heltec.display->drawString(0, 0, "Sending packet: " + String(counter) + " ID: " + String(id));
  Heltec.display->drawString(0, 10, "Temperature: " + String(temperature) + " °C");
  Heltec.display->drawString(0, 20, "Capacitive: " + String(capactive));
  Heltec.display->drawString(0, 30, "Pressure: " + String(pressure) + " hPa");
  Heltec.display->drawString(0, 40, "Approx. Altitude: " + String(altitude) + " m");
  Heltec.display->drawString(0, 50, "Humidity: " + String(humidity) + " %");
  Heltec.display->display();

  delay(2000); // wait for 2 seconds
}
