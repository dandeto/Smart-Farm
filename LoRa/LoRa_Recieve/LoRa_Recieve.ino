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

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet;
int ids = 1;
volatile bool rx = false;

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void LoRaData(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 15, "Received "+ packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
  Serial.println(packet);
}

void cbk(int packetSize) {
  if(packetSize == 2){ // request for ID
    LoRa.beginPacket();
    LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
    LoRa.print("ID ");
    LoRa.print(ids++);
    LoRa.endPacket();
    Serial.println("Sent ID" + String(ids-1));
  } else if(packetSize) {
    Serial.println("Got Data");
    packet ="";
    packSize = String(packetSize, DEC);
    for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
    rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
    LoRaData();
  }
}

void setup() { 
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, false /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.begin(115200);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("RS "); // reset
  LoRa.print(0); // init message - reset connection for anything that was previously connected
  LoRa.endPacket();
  Serial.println("RESET");
  
  LoRa.receive();
}

void loop() {
  // Delay - poll just in case there is a new connection
  int d = 0;
  while(++d < 1000) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) { cbk(packetSize);  }
    delay(10);
  }

  // request packet from each
  for(int i=1; i<ids; i++) {
    // prompt packet
    Serial.println("Request From ID " + String(i));
    LoRa.beginPacket();
    LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
    LoRa.print("RQ "); // request
    LoRa.print(i); // just print 1 as sanity check
    LoRa.endPacket();
    // wait for return...
    d=0;
    while(++d < 1000) {
      int packetSize = LoRa.parsePacket();
      if (packetSize) { cbk(packetSize);  }
      delay(10);
    }
  }
}