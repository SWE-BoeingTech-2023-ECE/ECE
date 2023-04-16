const int REED_PIN = 2;	// Pin connected to reed switch

void setup() {
	Serial.begin(9600);
	pinMode(REED_PIN, INPUT_PULLUP);	// Enable internal pull-up for the reed switch
}

void loop() {
	int proximity = digitalRead(REED_PIN); // Read the state of the switch
	
	// If the pin reads low, the switch is closed.
	if (proximity == LOW) {
		Serial.println("Switch closed");
	}
	else {
		Serial.println("Switch opened");
	}
}