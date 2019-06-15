#include "fanfriend.h"
#include <util/delay.h>

void InitPWM() {
	TCCR0A|=(1<<WGM00)|(1<<WGM01)|(1<<CS00)|(1<<COM0A1);
}

void BlinkLED() {
	LEDs_SetAllLEDs(0xFF);
	_delay_ms(125);
	LEDs_SetAllLEDs(0x00);
	_delay_ms(125);
}

int main(void) {
	SetupHardware();
	LEDs_Init();
	InitPWM();
	GlobalInterruptEnable();

	for (;;) {
        BlinkLED();
		// HID_Device_USBTask(&Keyboard_HID_Interface);
		// HID_Device_USBTask(&Mouse_HID_Interface);
		// USB_USBTask();
	}
}

void SetupHardware() {
    ;
}