 /*
  OLED pins to ESP32 GPIOs:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
*/

#include <Preferences.h>
#include "heltec.h"
#include "images.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_seesaw.h"

#define BAND 915E6  //you can set band here directly,e.g. 868E6,915E6
#define SEALEVELPRESSURE_HPA (1013.25)

unsigned long hardware_id = 0;
int id = 0;
long token = 0;
volatile long rx_token = 0;
volatile bool rx = false;
String type;
volatile int rx_id;
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
  type = "";
  type += (char)LoRa.read(); // type is first 2 bytes
  type += (char)LoRa.read();
  rx_id = LoRa.parseInt();
  if(type == "ID") rx_token = LoRa.parseInt();
  //Serial.println("Packet: " + type + " " + String(rx_id) + " " + String(token)); // debug
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

  // Get Hardware ID
  Preferences p;
  while (!p.begin("node", true)) {
    Heltec.display->drawString(0, 0, "Cannot Access NVM\n - This Board May Be Broken");
    Heltec.display->display();
    delay(100);
  }

  hardware_id = p.getULong("id", 0);
  if(hardware_id == 0) {
    Heltec.display->drawString(0, 0, "Board not provisioned.");
    Heltec.display->display();
    while(1);
  }

  p.end();

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
  
  LoRa.onReceive(getPacket); // ISR for when receive done
  //LoRa.onTxDone(onTxDone); // ISR for when transmission done
  LoRa.receive(); // put radio in receive mode
}

void loop() {
  while(!id) {
    delay(random(100,5000));
    Heltec.display->clear();
    Heltec.display->drawString(0, 0,  id ? "ID: " + String(id) : "Waiting for ID...");
    Heltec.display->display();
    // ask for id
    do {
      LoRa.beginPacket();
      LoRa.print("ID ");
      token = random(1,247483647);
      LoRa.print(token);
      LoRa.endPacket();
    } while(!LoRa.endPacket());
    LoRa.receive();
    Serial.println("Request ID");
    // poll for 2 seconds
    int count = 0;
    while(type != "ID" && ++count < 3000) {
      delay(10);
    }
    if(type == "ID" && token == rx_token) {
      Serial.println("Get ID");
      id = rx_id;
    }
  }

  // respond to request for data
  if(rx) {
    Serial.println("Packet: " + type + String(rx_id));
    if(type == "RQ" && rx_id == id) { // server request packet
      do {
        LoRa.beginPacket();
        LoRa.print("RS"); // packet type - response
        LoRa.print(id);
        LoRa.print(",");
        LoRa.print(hardware_id);
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
      
      // flash LED to indicate response
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
      
    } else if(type == "RS" && rx_id == 0) { // server reset
      Serial.println("RESET");
      id = 0; // reset
      do { // request id
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
