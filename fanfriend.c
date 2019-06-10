#include "fanfriend.h"
#include <util/delay.h>

int main(void) {
	SetupHardware();
    LEDs_Init();
	GlobalInterruptEnable();

	for (;;) {
        LEDs_SetAllLEDs(0xFF);
        _delay_ms(125);

        LEDs_SetAllLEDs(0x00);
        _delay_ms(125);
		// HID_Device_USBTask(&Keyboard_HID_Interface);
		// HID_Device_USBTask(&Mouse_HID_Interface);
		// USB_USBTask();
	}
}

void SetupHardware() {
    ;
}