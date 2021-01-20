/*
 Name:		Work_Automator.ino
 Created:	1/8/2021 5:22:41 PM
 Author:	jimmy
*/

#include <USBComposite.h>

USBHID HID;
HIDKeyboard Keyboard(HID);

#define potPin PA3				//potentiometer

#define eastButt	PB15		//for some reason only certain pins would work right, not sure why
#define westButt	PB4			//hopefully all these work.
#define northButt	PB14		//confirmed works
#define southButt	PB5

//LEDs: left and right RGB
#define leftRed		PA0			
#define leftBlue	PA2			//RGB pins need the PWM pins 
#define leftGreen	PA1

#define rightRed	PB9			//150 ohm on red and 22 for B/G
#define rightBlue	PB7
#define rightGreen	PB8

//button states used for toggle
bool state_mute = 0;
bool state_meet = 0;

bool eastButt_new;
bool westButt_new;
bool northButt_new;
bool southButt_new;


bool eastButt_old = 1;
bool westButt_old = 1;		//current_state
bool northButt_old = 1;
bool southButt_old = 1;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 200;    // the debounce time; increase if the output flickers


volatile bool buttPress_flag	= LOW;		//flag set if any buttons are pressed
volatile bool northPress_flag	= LOW;
volatile bool eastPress_flag	= LOW;
volatile bool southPress_flag	= LOW;
volatile bool westPress_flag	= LOW;


/*Physical Layout:
*				  dial
* leftColor						rightColor
*				northButt
* 
* westButt						eastButt
* 
*				southButt 			
*/

void setup() {
	Serial.begin(9600);					

	pinMode(potPin, INPUT_ANALOG);

	pinMode(leftRed, PWM);
	pinMode(leftBlue, PWM);
	pinMode(leftGreen, PWM);

	pinMode(rightRed, PWM);
	pinMode(rightBlue, PWM);
	pinMode(rightGreen, PWM);
	
	pinMode(northButt, INPUT_PULLUP);
	pinMode(southButt, INPUT_PULLUP);
	pinMode(eastButt, INPUT_PULLUP);
	pinMode(westButt, INPUT_PULLUP);

	

	//this stuff was from the example in rogerclark
	HID.begin(HID_KEYBOARD);
	while (!USBComposite);
	Keyboard.begin();		// useful to detect host capslock state and LEDs

	

	if (digitalRead(northButt) == LOW) {	//only show the welcome message and git link if the button is pressed as it is plugged in / reset. 
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
	case 0: {									//Teams controller
		leftColor(7000, 8000, 0);					//purple = teams mode, keep this always on in this mode

		if (buttPress_flag) {
			buttPress_detect_case0();
			buttPress_flag = LOW;
		}
		if (state_meet) {
			rightColor(20000, 0, 6300);
		}
		if (!state_meet) {
			rightColor(0, 0, 0);
		}
		if (state_mute) {
			rightColor(20000, 0, 0);
		}

		break; //end case 0
	} //not sure why but this bracket needed to be here to work at some point...

	case 1:								//JDE login
		leftColor(0, 0, 100);			//green
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

		break; //end case 1


	case 2:											//Office Tools
		leftColor(10, 60, 10);

		if (state_meet) {				//if dial setting changed during meeting
			rightColor(75, 0, 25);
		}
		if (state_mute) {
			rightColor(75, 0, 0);		//keep red light on when unmuted and changing dial setting
		}
		if (buttPress_flag) {
			buttPress_detect_case2();
			buttPress_flag = LOW;

			//TBD the actual functions
			//Unlock doc (alt 6), add/delete table rows (ctr +/-), format painter
		
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
		if (buttPress_flag) {
			buttPress_detect_case3();
			buttPress_flag = LOW;
		}

			//TBD the actual functions
		
		break;
	default:
		leftColor(75, 75, 75);		//not sure if a default is necessary
		rightColor(75, 75, 75);

	}
	delay(100);		//apparently this delay is critical...
}


//Set the LED colors with these functions
//using pwmWrite instead of analogWrite
//values range 0 - 65535
void leftColor(int leftRed_value, int leftBlue_value, int leftGreen_value) { //I'm a little dyslexic, the green should've gone before blue for a RGB LED but too late now.
	pwmWrite(leftRed, leftRed_value);
	pwmWrite(leftBlue, leftBlue_value);
	pwmWrite(leftGreen, leftGreen_value);
}

void rightColor(int rightRed_value, int rightBlue_value, int rightGreen_value) {
	pwmWrite(rightRed, rightRed_value);
	pwmWrite(rightBlue, rightBlue_value);
	pwmWrite(rightGreen, rightGreen_value);
}