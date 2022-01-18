#include "heltec.h" 
#include "images.h"
#include "Node.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet;
NodeManager nodeManager;
volatile bool rx = false;
volatile int packetSize = 0;

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
  String type = "";
  type += (char)LoRa.read(); // packet type is first 2 bytes
  type += (char)LoRa.read();

  if(type == "ID") { // request for ID
    // grab token
    long token = LoRa.parseInt();
    if(token) {
      //Serial.println(token); // debug
      int id = nodeManager.add();
      LoRa.beginPacket();
      LoRa.print("ID ");
      LoRa.print(id);
      LoRa.print(" ");
      LoRa.print(token); // send token in reponse
      LoRa.endPacket();
    }
  } else if(packetSize) {
    int id = LoRa.parseInt(); 
    nodeManager.response(id);
    packet = String(id);
    packSize = String(packetSize, DEC);
    for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
    rssi = "RSSI " + String(LoRa.packetRssi(), DEC);
    LoRaData();
  }
  //nodeManager.print(); // debug
}

// This ISR will run even during a delay function
void receiveISR(int size) {
  if(size) rx = true;
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
  
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("RS "); // reset
  LoRa.print(0); // init message - reset connection for anything that was previously connected
  LoRa.endPacket();
  //Serial.println("RESET"); // debug

  //LoRa.onReceive(receiveISR);
  LoRa.receive();
}

void loop() {
  // 10s delay - poll to recieve data
  int d = 0;
  while(++d < 1000) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) { cbk(packetSize);  }
    delay(10);
  }

  // request packet from each sensor node
  int id = nodeManager.next();
  if(id) {
    do {
      //Serial.println("Request From ID " + String(id)); // debug
      LoRa.beginPacket();
      LoRa.print("RQ " + String(id));
    } while(!LoRa.endPacket());
    nodeManager.request(id);
  }
}
