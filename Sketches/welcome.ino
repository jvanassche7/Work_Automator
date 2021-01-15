// Welcome message to run only once after boot/reset and only iff the north button is held 

void welcome() {
	//*****add function to flash the LEDs while instructions are written

	leftColor(100, 0, 0);
	rightColor(100, 0, 0);				//set both to Red for now..
	
	delay(2000);
	Keyboard.press(KEY_LEFT_GUI);		//press windows button to open start menu
	Keyboard.release(KEY_LEFT_GUI);
	delay(2000);						//wait some sec to let start menu open

	Keyboard.println("notepad ");		//search notepad
	delay(500);							//give time for it to find it
	Keyboard.press(KEY_RETURN);			//open notepad
	Keyboard.release(KEY_RETURN);
	delay(8000);						//wait 8 seconds for notepad to open/load... might open notepad++ if its available

	//start welcome message and instructions for use. 
	Keyboard.println("_____     Welcome to the Work Automator     _____");
	delay(250);
	Keyboard.println("Brought to you by J4 Laboratories...");
	delay(500);
	Keyboard.println("...with consulting considerations by CNC Designs");
	delay(250);
	Keyboard.println("<Firmware: 1.0>");
	Keyboard.println("<Hardware: 1.0>");
	delay(250);

	Keyboard.println("The Work Automator is designed and programmed to streamline work in Microsoft Teams, Office, and JDE, with the capability for rapid and unlimited growth. It consists of a Dial to set the functioning Mode and several buttons to carry out specified tasks. It also contains indicator LEDs. And Chris sucks");
	Keyboard.println("INSTRUCTIONS FOR USE:");
	Keyboard.println("Mode A: Teams");
	Keyboard.println(" ");
	Keyboard.println("Button 1: Activate Meeting		Button 2: Toggle Mute");
	Keyboard.println("Always start a meeting by manually muting. Press button 1 to turn on the White LED indicating you are in a meeting and activating button 2.Once in the meeting, press button 2 to unmute. This will activate the Red LED indicating a hot mic. Press button 2 again to mute, which deactivates the Red Live LED.");
	Keyboard.println(" ");
	Keyboard.println("************TBD*************");
	//JDE login
		//"INQUSR", TAB, "TCVSINQUSR", ENTER, wait few sec, repeat, ENTER (to get to main menu)
	//add/delete rows = ctr +/- (word/excel)
	//format painter?


	leftColor(50, 50, 50);			//set left to white and turn right off
	rightColor(0, 0, 0);

}
}
