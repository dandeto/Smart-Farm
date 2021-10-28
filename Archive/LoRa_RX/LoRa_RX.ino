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
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//////////////////////////////////////////////////
// Variables
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
  // Eventually InterpretCommand will need to be
  // called from ISR that exits sleep mode
  InterpretCommand();
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
  Serial.print("Command: "); Serial.println(Adr,16);
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
      // Set sampling frequency
      Serial.println("Set sampling frequency");
      break;

    case 0x02:
      // Get latest reading
      Serial.println("Get latest reading");
      break;

    case 0x03:
      // Get last n readings
      Serial.println("Get last n readings");
      break;

    default:
      // WTF just happened
      Serial.println("MAYDAY");
  }
}