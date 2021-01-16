/*
 Name:		Work_Automator.ino
 Created:	1/8/2021 5:22:41 PM
 Author:	jimmy
*/

#include <USBComposite.h>

USBHID HID;
HIDKeyboard Keyboard(HID);

#define potPin PA3			//potentiometer

#define eastButt	PB15		//for some reason only certain pins would work right, not sure why
#define westButt	PB4			//hopefully all these work.
#define northButt	PB14		//confirmed works
#define southButt	PB5

//LEDs: left and right RGB
#define leftRed		PA0			
#define leftBlue	PA2				//RGB pins need the PWM pins 
#define leftGreen	PA1

#define rightRed	PB9			// 150 ohm on red and 22 for B/G
#define rightBlue	PB7
#define rightGreen	PB8

//button states used for toggle
bool state_mute = 0;
bool state_meet = 0;

bool eastButt_new;
bool westButt_new;
bool northButt_new;


bool eastButt_old = 1;
bool westButt_old = 1;		//current_state
bool northButt_old = 1;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 200;    // the debounce time; increase if the output flickers


volatile bool buttPress_flag	= LOW;		//flag set if any buttons are pressed
volatile bool northPress_flag	= LOW;
volatile bool eastPress_flag	= LOW;
volatile bool southPress_flag	= LOW;
volatile bool westPress_flag	= LOW;


/*Physical Button Layout:
				northButt
 
  westButt						eastButt

				southButt
*/

void setup() {
	Serial.begin(9600);					

	pinMode(potPin, INPUT);

	pinMode(leftRed, OUTPUT);
	pinMode(leftBlue, OUTPUT);
	pinMode(leftGreen, OUTPUT);

	pinMode(rightRed, OUTPUT);
	pinMode(rightBlue, OUTPUT);
	pinMode(rightGreen, OUTPUT);
	
	pinMode(northButt, INPUT_PULLUP);
	pinMode(southButt, INPUT_PULLUP);
	pinMode(eastButt, INPUT_PULLUP);
	pinMode(westButt, INPUT_PULLUP);

	

	//this stuff was from the example in rogerclark
	HID.begin(HID_KEYBOARD);
	while (!USBComposite);
	Keyboard.begin();		// useful to detect host capslock state and LEDs

	

	if (digitalRead(northButt) == LOW) {	//only show the welcome message if the button is pressed as it is plugged in / reset. 
		welcome();
	}

	//Interrupts, FALLING because they are normally High and we will only care about when it first drops when its pressed. 
	//putting these at the end of the setup so that the welcome message will still work when north butt is held 
	attachInterrupt(digitalPinToInterrupt(northButt), northButt_ISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(eastButt), eastButt_ISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(westButt), westButt_ISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(southButt), southButt_ISR, FALLING);
}

void loop() {

	int potReading = analogRead(potPin);
	int dial = map(potReading, 0, 4095, 0, 3);		//map takes the reading and the range of the ADC, then the range you want it broken into. So 4095 / 4 = setting range

	switch (dial) {
	case 0: {										//Teams controller
		leftColor(70, 80, 0);					//purple = teams mode, keep this always on in this mode

		if (buttPress_flag) {
			buttPress_detect_case0();
			buttPress_flag = LOW;
		}
		if (state_meet) {
			rightColor(75, 0, 25);
		}
		if (!state_meet) {
			rightColor(0, 0, 0);
		}
		if (state_mute) {
			rightColor(75, 0, 0);
		}
		
		

		break; //end case 0
	}

	case 1:											//JDE login
		leftColor(0, 0, 100);
		rightColor(0, 0, 0);

		if (state_meet) {				//if dial setting changed during meeting
			rightColor(75, 0, 25);
		}
		if (state_mute) {
			rightColor(75, 0, 0);		//keep red light on when unmuted and changing dial setting
		}

		if (buttPress_flag) {
			buttPress_detect_case1();
			buttPress_flag = LOW;
		}

		if (digitalRead(westButt) == LOW) {
			//print Login Info
			rightColor(70, 150, 0);
			delay(500);
		}
		break;
	case 2:											//Office Tools
		leftColor(10, 60, 10);

		if (state_meet) {				//if dial setting changed during meeting
			rightColor(75, 0, 25);
		}
		if (state_mute) {
			rightColor(75, 0, 0);		//keep red light on when unmuted and changing dial setting
		}

		if (digitalRead(southButt) == LOW) {
			rightColor(0, 50, 50);
			delay(500);
		}
		break;
	case 3:											//TBD
		leftColor(75, 0, 75);

		if (state_meet) {				//if dial setting changed during meeting
			rightColor(75, 0, 25);
		}
		if (state_mute) {
			rightColor(75, 0, 0);		//keep red light on when unmuted and changing dial setting
		}

		if (digitalRead(northButt) == LOW) {
			rightColor(75, 0, 75);
		}
		break;
	default:
		leftColor(75, 75, 75);
		rightColor(75, 75, 75);

	}
	delay(100);
}


//Set the LED colors with these functions
void leftColor(int leftRed_value, int leftBlue_value, int leftGreen_value) {
	analogWrite(leftRed, leftRed_value);
	analogWrite(leftBlue, leftBlue_value);
	analogWrite(leftGreen, leftGreen_value);
}

void rightColor(int rightRed_value, int rightBlue_value, int rightGreen_value) {
	analogWrite(rightRed, rightRed_value);
	analogWrite(rightBlue, rightBlue_value);
	analogWrite(rightGreen, rightGreen_value);
}