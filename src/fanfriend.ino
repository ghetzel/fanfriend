#include <Arduino.h>
#include "config.h"
#include "cli.h"

// Pin I/O Mappings
//  This is where the fan INPUTS (signal read from the motherboard declaring what the HP firmware
//  *wants* the speed to be) are setup, as well as the fan OUTPUTS (what we tell the fans the speed
//  *will* be.)
// -------------------------------------------------------------------------------------------------
static int FanInputPins[MAX_FANS] = {
  0,  // Arduino Analog In 0    (ATMega32U4 F7)
  1,  // Arduino Analog In 1    (ATMega32U4 F6)
  2,  // Arduino Analog In 2    (ATMega32U4 F5)
  3,  // Arduino Analog In 3    (ATMega32U4 F4)
  4,  // Arduino Analog In 4    (ATMega32U4 F1)
  5,  // Arduino Analog In 5    (ATMega32U4 F0)
};

static int FanOutputPins[MAX_FANS] = {
  3,  // Arduino Digital Pin 3  (ATMega32U4 D0)
  2,  // Arduino Digital Pin 2  (ATMega32U4 D1)
  0,  // Arduino Digital Pin 0  (ATMega32U4 D2)
  1,  // Arduino Digital Pin 1  (ATMega32U4 D3)
  4,  // Arduino Digital Pin 4  (ATMega32U4 D4)
  12, // Arduino Digital Pin 12 (ATMega32U4 D6)
};

// Change the duty cycle on the numbered fan. Clamps the duty factor
// to [0, 255] and validates the fan index.  Will return 0 on success
// and n<0 on failure.
int SetFanDuty(int fan, float dutyFactor) {
  if (fan < 0 || fan > MAX_FANS) {
    return -1;
  } else if (dutyFactor < 0) {
    dutyFactor = 0;
  } else if (dutyFactor > 255) {
    dutyFactor = 255;
  } else if (FanCurrentDutyCycle[fan] == dutyFactor) {
    return 0;
  }

  FanCurrentDutyCycle[fan] = dutyFactor;
  return 0;
}

void setup() {
  // Set initial fan states
  for(int i = 0; i < MAX_FANS; i++) {
    SetFanDuty(1, INIT_FAN_SPEED);
    pinMode(FanOutputPins[i], OUTPUT);

    // set the initial value we're going to drive the fans at
    FanCurrentDutyCycle[i] = INIT_FAN_SPEED;

    // initialize the "what the motherboard wants us to do" array
    FanSystemDutyCycle[i] = 0;
  }

  // Setup pins
  pinMode(LED_PIN, OUTPUT); // green BOOT LED
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(9600);
  while(!Serial) { ; }
  CliInit();
}

void loop() {
  for(int i = 0; i < MAX_FANS; i++) {
    analogWrite(FanOutputPins[i], 255 * FanCurrentDutyCycle[i]);
  }

  if (CliReadLine(CliLine)) {
    int res = CliExec(CliLine);

    Serial.println("");

    if (res >= 0) {
      Serial.println("OK");
    } else {
      Serial.println("ERR");
    }

    CliReset();
  }
}