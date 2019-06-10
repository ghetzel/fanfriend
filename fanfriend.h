#ifndef FANFRIEND_H_
#define FANFRIEND_H_

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
// #include "Descriptors.h"
#include <LUFA/Drivers/Board/LEDs.h>
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

void SetupHardware();

#endif // FANFRIEND_H_