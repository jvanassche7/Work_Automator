/*
 Name:		Work_Automator.ino
 Created:	1/8/2021 5:22:41 PM
 Author:	jimmy
*/

#include <USBComposite.h>

USBHID HID;
HIDKeyboard Keyboard(HID);

int muteButton = PB5;		//for some reason only certain pins would work right, not sure why
int meetButton = PA3;

int liveLED = PA10;
int meetLED = PB1;

int state_Meet = 0;
int state_Mute = 0;

int buttonNew_Meet;
int buttonNew_Mute;

int buttonOld_Meet = 1;
int buttonOld_Mute = 1;


void setup() {
	Serial.begin(9600);					//not sure if this is necessary but that one dude in the tutorial did it
	pinMode(muteButton, INPUT_PULLUP);
	pinMode(meetButton, INPUT_PULLUP);

	pinMode(liveLED, OUTPUT);
	pinMode(meetLED, OUTPUT);

	//this stuff was from the example in rogerclark
	HID.begin(HID_KEYBOARD);
	while (!USBComposite);
	Keyboard.begin(); // useful to detect host capslock state and LEDs
}

void loop() {
	buttonNew_Mute = digitalRead(muteButton);
	if (buttonOld_Mute == 0 && buttonNew_Mute == 1 && state_Meet == 1) { //if mute button state changed and the power is currently on
		if (state_Mute == 0) {					//if mute was already off

			digitalWrite(liveLED, HIGH);

			Keyboard.press(KEY_LEFT_CTRL);		//modifier/non printing keys are in USBHID.h
			Keyboard.press(KEY_LEFT_SHIFT);
			Keyboard.press('m');

			Keyboard.release(KEY_LEFT_CTRL);
			Keyboard.release(KEY_LEFT_SHIFT);
			Keyboard.release('m');

			/*
			Development use only
			Keyboard.press(KEY_LEFT_CTRL);
			Keyboard.press('b');
			Keyboard.release(KEY_LEFT_CTRL);
			Keyboard.release('b');
			*/

			delay(100);
			state_Mute = 1;						//says mute is on
		}
		else {									//if mute was already on, this turns it off
			digitalWrite(liveLED, LOW);
			state_Mute = 0;

			Keyboard.press(KEY_LEFT_CTRL);
			Keyboard.press(KEY_LEFT_SHIFT);
			Keyboard.press('m');
			Keyboard.release(KEY_LEFT_CTRL);
			Keyboard.release(KEY_LEFT_SHIFT);
			Keyboard.release('m');

			/*
			Development use only
			Keyboard.press(KEY_LEFT_CTRL);
			Keyboard.press('b');
			Keyboard.release(KEY_LEFT_CTRL);
			Keyboard.release('b');
			*/
		}
	}
	buttonOld_Mute = buttonNew_Mute;
	delay(25);

	//yes chris, I'm aware there should be button debounce stuff, but it works as is and I dont feel like it. maybe next rev...
	// The only reason it works right now is because of the delay at line 59. That's essentially your debounce as it makes the 
	// program wait 100ms after it does some stuff. If you took that dealy out, or made it much shorter, than it would probably bounce
	// a lot. So you technically did debounce it, just poorly.
	// Use #define to set your button/LED names you heathen. See below example
	// #define muteButton PB5
	// digitalRead(muteButton) and other stuff would remain unchanged.
	// Variables that are boolean and will only be 0/1 should be defined as such. 
	// bool state_Meet = LOW; or bool state_Meet = 0; are better since defining as integer will tell the compiler it can be any 32bit
	// number when in reality it's only 0 or 1. Not a huge deal, but takes up more space than needed.
	// buttonNew_Mute (and others) make me want to jump off the roof. Lowercase after the underscore.
	// Protip for if statements.
	// if (buttonOld_Mute == 0 && buttonNew_Mute == 1 && state_Meet ==1) can be reduced to 
	// if (!buttonOld_Mute && buttonNew_Mute && state_Meet). If it's just boolean you can say just the variable yes or no. 
	// ! in front of buttonOld_Mute means not 1.
	// Overall not bad and does the job


	//this is the main power button/LED 
	//this has to be on for the mute button to work. 
	buttonNew_Meet = digitalRead(meetButton);
	if (buttonOld_Meet == 0 && buttonNew_Meet == 1) {
		if (state_Meet == 0) {
			digitalWrite(meetLED, HIGH);


			delay(100);
			state_Meet = 1;
		}
		else {
			digitalWrite(meetLED, LOW);
			state_Meet = 0;
			digitalWrite(liveLED, LOW); // cuts both LEDs if turn power off
			state_Mute = 0;
		}
	}
	buttonOld_Meet = buttonNew_Meet;
	delay(25);



}