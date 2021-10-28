#include <SPI.h>
#include <LoRa.h>

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

void setup() {
  // Initialize serial connection
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa TX Example");

  // Initialize LoRa
  Serial.print("Starting LoRa.");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
  while (!LoRa.begin(915E6)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Sucess!");
}

void loop() {
  // Send test packet
  Serial.println("Sending test packet...");
  LoRa.beginPacket();
  LoRa.print("Hello World!");
  LoRa.endPacket();
  delay(5000);
}
