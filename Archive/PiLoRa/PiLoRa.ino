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

  // Initialize LoRa
  Serial.print("Starting LoRa.");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
  while (!LoRa.begin(915E6)) {
    delay(500);
  }
}

void loop() {
  // Pi->LoRa
  if (Serial.available()) {
    LoRa.beginPacket();
    while (Serial.available()) {
      LoRa.write(Serial.read());
    }
    LoRa.endPacket();
  }

  // LoRa->Pi
  if (LoRa.parsePacket()) {
    while (LoRa.available()) {
      Serial.write(LoRa.read());
    }
  }
}
