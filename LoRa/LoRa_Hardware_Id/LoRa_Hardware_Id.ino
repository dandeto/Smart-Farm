#include <Preferences.h>
#define ID 2

void setup() {
	Serial.begin(115200);

	// Initialize interface to NVS
	Preferences p;
	// Open the node namespace
	if(!p.begin("node", false)) {
		Serial.println("This board is broken");
		return;
	}

	unsigned long id = p.getULong("id", 0);

	if(id != 0) Serial.println("ID was: " + String(id));

	p.putULong("id", ID);
	Serial.println("ID is now: " + String(p.getULong("id", 0)));
	p.end();
}

void loop() {}
