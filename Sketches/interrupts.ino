//Interrupts used for Buttons

void northButt_ISR() {
	buttPress_flag = HIGH;
	northPress_flag = HIGH;
}

void eastButt_ISR() {
	buttPress_flag = HIGH;
	eastPress_flag = HIGH;
}

void westButt_ISR() {
	buttPress_flag = HIGH;
	westPress_flag = HIGH;
}

void southButt_ISR() {
	buttPress_flag = HIGH;
	southPress_flag = HIGH;
}

