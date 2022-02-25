#include "heltec.h"
#include "HardwareSerial.h"
#include "images.h"
#include "Node.h"

#define BAND             915E6 // set frequency band here,e.g. 868E6,915E6
#define sleep_delay      10    // these are all in seconds
#define node_sleep_delay 5
#define request_delay    8

NodeManager nodeManager;

void displayPacket(String rssi, int packetSize, String packet){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 15, "Received "+ String(packetSize, DEC) + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);
  Heltec.display->display();
  Serial.println(packet);
}

void readPacket(int packetSize) {
  String type = "";
  type += (char)LoRa.read(); // packet type is first 2 bytes
  type += (char)LoRa.read();

  if(type == "ID") { // request for ID
    // grab token
    long token = LoRa.parseInt();
    if(token) {
      //Serial.println("Recieved token " + String(token)); // debug
      int id = nodeManager.add();
      delay(1000);
      do {
        LoRa.beginPacket();
        LoRa.print("ID ");
        LoRa.print(id);
        LoRa.print(" ");
        LoRa.print(token);
      } while(!LoRa.endPacket());
      LoRa.receive();
      //Serial.println("Sent Packet: ID " + String(id) + " " + String(token)); //debug
    }
  } else if(packetSize) {
    int id = LoRa.parseInt(); // get network id
    LoRa.read(); // throw away comma
    nodeManager.response(id);
    String packet;
    for (int i = 0; i < packetSize; i++) packet += (char) LoRa.read();
    String rssi = "RSSI " + String(LoRa.packetRssi(), DEC);
    displayPacket(rssi, packetSize, packet);
  }
  //nodeManager.print(); // debug
}

void poll(int _delay) {
  // poll for number of seconds specified by _delay 
  int d = 0;
  while(++d < _delay*100) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) readPacket(packetSize);
    delay(10);
  }
}

void setup() { 
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, false /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.begin(115200);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
  delay(1500);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("RS "); // tell nodes to reset
  LoRa.print(0);
  LoRa.endPacket();
  LoRa.receive();
}

void loop() {
  poll(10); // poll for 10 seconds

  // Request packet from each sensor node
  int id = nodeManager.next();
  while(id) {
    do {
      Serial.println("Request From ID " + String(id)); // debug
      LoRa.beginPacket();
      LoRa.print("RQ " + String(id));
    } while(!LoRa.endPacket());
    nodeManager.request(id);
    poll(8);
    id = nodeManager.next();
  }

  // Put all nodes to sleep
  do {
    LoRa.beginPacket();
    LoRa.print("SL");
    LoRa.print(node_sleep_delay);
  } while(!LoRa.endPacket());
}
