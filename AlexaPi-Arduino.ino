#include <Arduino.h>

const byte PIN_LED_PLAYBACK = 2;
const byte PIN_LED_RECORDING = 3;
const byte PIN_BUTTON_TRIGGER = 5;

enum states {
	IDLE,
	PLAYBACK,
	RECORDING,
	PROCESSING,  
};

states state = IDLE;
states statePrevious = IDLE;

byte buttonState = false;
byte buttonStatePrevious = false;

char serialData;

void setup() 
{
	Serial.begin(9600);

	pinMode(PIN_LED_PLAYBACK, OUTPUT);
	pinMode(PIN_LED_RECORDING, OUTPUT);
	pinMode(PIN_BUTTON_TRIGGER, INPUT_PULLUP);

	// wait for serial port to connect. Needed for Leonardo only
	while (!Serial) {
		;
	}
}

void loop() 
{
	if (Serial.available()) {
		statePrevious = state;
		
		serialData = (char) Serial.read();
		
		if (serialData == 'l') {
			state = PLAYBACK;
		} else if (serialData == 'r') {
			state = RECORDING;
		} else if (serialData == 'p') {
			state = PROCESSING;
		} else if (serialData == 'L' || serialData == 'R' || serialData == 'P') {
			state = IDLE;
		}
	}

	if (statePrevious != state) {
		digitalWrite(PIN_LED_PLAYBACK, 0);
		digitalWrite(PIN_LED_RECORDING, 0);

		switch (state) {
			case PLAYBACK:
				digitalWrite(PIN_LED_PLAYBACK, 1);
				break;
			case RECORDING:
				digitalWrite(PIN_LED_RECORDING, 1);
				break;
			case PROCESSING:
				digitalWrite(PIN_LED_PLAYBACK, 1);
				digitalWrite(PIN_LED_RECORDING, 1);
				break;
			default:
				break;
		}
	}

	// since we use an internal PULLUP resistor, we have to invert the state
	buttonStatePrevious = buttonState;
	buttonState = !digitalRead(PIN_BUTTON_TRIGGER);
	if (buttonState != buttonStatePrevious) {
		if (buttonState) {
			Serial.write('t');
		} else {
			Serial.write('T');
		} 
	}

}
