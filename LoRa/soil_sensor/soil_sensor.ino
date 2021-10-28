#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

void setup() {
  Serial.begin(115200);

  Serial.println("Init Serial");
  
  while (!ss.begin(0x36)) {
    Serial.println("ERROR! Seesaw not found");
    delay(100);
  }
  Serial.print("Seesaw started! Version: ");
  Serial.println(ss.getVersion(), HEX);
}

void loop() {
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);
  delay(100);
}
