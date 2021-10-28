//////////////////////////////////////////////////
// Includes
//////////////////////////////////////////////////
#include <SPI.h>
#include <LoRa.h>
#include "EEPROM.h"

//////////////////////////////////////////////////
// Defines
//////////////////////////////////////////////////
#define EEPROM_SIZE 512
#define uS_TO_S_FACTOR 1000000ULL
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//////////////////////////////////////////////////
// Variables/Sensors
//////////////////////////////////////////////////
uint8_t ADR_DEF = 0xFF;

//////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////
void InterpretCommand(void);

//////////////////////////////////////////////////
// Setup
//////////////////////////////////////////////////
void setup() {
  // Initialize serial connection
  Serial.begin(115200);
  while (!Serial);

  // Initialize LoRa
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
  while (!LoRa.begin(915E6)) {
    delay(500);
  }

  // Initialize EEPROM
  if (!EEPROM.begin(EEPROM_SIZE)) {
    for(;;);
  }

  // Load in address from EEPROM
  ADR_DEF = EEPROM.read(0);

  // Display startup message
  Serial.println("Node start success");
  Serial.print("Device Address: "); Serial.println(ADR_DEF,16);
}

//////////////////////////////////////////////////
// Loop
//////////////////////////////////////////////////
void loop() {
  // Normal Program Operation
  InterpretCommand();

  // Testing
  /*
  double temp, hum, pres, soil;
  temp = bme.readTemperature(); 
  hum = bme.readHumidity(); 
  pres = bme.readPressure()/100.0F;
  soil = (double) ss.touchRead(0);
  Serial.print(temp); Serial.print(",");
  Serial.print(hum); Serial.print(",");
  Serial.print(pres); Serial.print(",");
  Serial.print(soil); Serial.println("");
  delay(1000);
  */
}

//////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////
void InterpretCommand() {
  // Return if nothing is there
  if (!LoRa.parsePacket())
    return;
  
  // Read in wake word
  uint16_t WakeWord = 0;
  *((uint8_t*) &(WakeWord)+1) = LoRa.read();
  *((uint8_t*) &WakeWord) = LoRa.read();
  Serial.println(WakeWord,16);
  if (WakeWord != 0xBEEF) {
    while (LoRa.available()) {
      LoRa.read();
    }
    return;
  }
  Serial.println("Wake word match!");

  // Read in device address
  uint8_t Adr = LoRa.read();
  
  if (Adr != ADR_DEF) {
    while (LoRa.available()) {
      LoRa.read();
    }
    return;
  }

  // Read in command type
  uint8_t cmd = LoRa.read();
  Serial.print("Adress: "); Serial.println(Adr,16);
  switch (cmd) {
    case 0x00:
      // Pair mode (change address)
      Serial.println("Pair Mode");
      uint8_t New_Adr;
      New_Adr = LoRa.read();
      EEPROM.write(0, New_Adr);
      EEPROM.commit();
      ADR_DEF = New_Adr;
      Serial.print("Address changed to ");
      Serial.println(New_Adr,16);
      break;

    case 0x01:
      // Get data
      double temp, hum, pres, soil;
      temp = 25.25; 
      hum = 96.2457; 
      pres = 400;
      soil = 70.2;
      Serial.println("Data acquired:");
      Serial.print("Temp: "); Serial.println(temp);
      Serial.print("Hum: "); Serial.println(hum);
      Serial.print("Pres: "); Serial.println(pres);
      Serial.print("Soil: "); Serial.println(soil);
      LoRa.beginPacket();
      LoRa.write((uint8_t*) &temp,8); 
      LoRa.write((uint8_t*) &hum,8); 
      LoRa.write((uint8_t*) &pres,8);
      LoRa.write((uint8_t*) &soil,8);
      LoRa.endPacket();
      break;

    case 0x02:
      // Go to sleep
      Serial.println("Sleep mode command");
      uint16_t SleepDurationS;
      *((uint8_t*) &(SleepDurationS)+1) = LoRa.read();
      *((uint8_t*) &SleepDurationS) = LoRa.read();
      Serial.println(SleepDurationS);
      esp_sleep_enable_timer_wakeup(SleepDurationS * uS_TO_S_FACTOR);
      Serial.println("Going to sleep...");
      esp_deep_sleep_start();
      break;
  }
}
