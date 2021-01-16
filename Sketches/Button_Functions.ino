//Button Functions for each dial setting

// ===============================================================================================
//CASE 0

void buttPress_detect_case0() {
	if (northPress_flag) {
		northPress_flag = LOW;

	}

	if (eastPress_flag) {
		eastButt_new = digitalRead(eastButt);
		lastDebounceTime = millis();	//timestamp for debounce
		while ((lastDebounceTime + debounceDelay) >= millis()) {	//while still in debounce window
			eastPress_flag = LOW;									//keep this button from being flagged as pressed
		}
		//state_mute = !state_mute;		//toggle state_mute

		if (eastButt_old && !eastButt_new && state_meet) {
			if (!state_mute) {
				rightColor(75, 0, 0);
				Keyboard.press(KEY_LEFT_CTRL);		//modifier/non printing keys are in USBHID.h
				Keyboard.press(KEY_LEFT_SHIFT);
				Keyboard.press('m');

				Keyboard.releaseAll();				//release all pressed keys

				state_mute = 1;
			}
			else {
				rightColor(75, 0, 25);
				state_mute = 0;

				Keyboard.press(KEY_LEFT_CTRL);		//modifier/non printing keys are in USBHID.h
				Keyboard.press(KEY_LEFT_SHIFT);
				Keyboard.press('m');

				Keyboard.releaseAll();				//release all pressed keys
			}
		}
	}


	if (westPress_flag) {
		westButt_old = digitalRead(westButt);
		lastDebounceTime = millis();	//timestamp for debounce
		while ((lastDebounceTime + debounceDelay) >= millis()) {	//while still in debounce window
			westPress_flag = LOW;									//keep this button from being flagged as pressed

		}
		if (digitalRead(westButt) == HIGH) {
			state_meet = !state_meet;		//toggle state_meet
		}
		//state_meet = !state_meet;		//toggle state_meet
		if (state_meet) {				//state meet is toggled in button_functions
			rightColor(75, 0, 25);
			
		}
		if (!state_meet) {							//turns the meeting light off
			rightColor(0, 0, 0);
			//state_meet = 0;
			state_mute = 0;				//turn mute light off when meeting ends
		}

	}


	if (southPress_flag) {
		southPress_flag = LOW;

	}
}


// ===========================================================================================================
//CASE 1

void buttPress_detect_case1() {

	if (northPress_flag) {
		northButt_old = digitalRead(northButt);
		lastDebounceTime = millis();	//timestamp for debounce
		while ((lastDebounceTime + debounceDelay) >= millis()) {	//while still in debounce window
			northPress_flag = LOW;									//keep this button from being flagged as pressed
		}

		northButt_new = digitalRead(northButt);
		if (northButt_old != northButt_new) {
			rightColor(255, 0, 125);
			Keyboard.print("INQUSR");
			delay(250);
			Keyboard.press(KEY_TAB);
			delay(10);
			Keyboard.release(KEY_TAB);
			delay(100);
			Keyboard.print("TCVSINQUSR");
			delay(50);
			Keyboard.press(KEY_RETURN);
			delay(10);
			Keyboard.release(KEY_RETURN);
			rightColor(0, 0, 0);

		}
		
	
	}


	if (eastPress_flag) {
		eastPress_flag = LOW;
	}


	if (westPress_flag) {
		westPress_flag = LOW;
	}


	if (southPress_flag) {
		southPress_flag = LOW;

	}
}