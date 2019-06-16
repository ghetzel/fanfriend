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
int SetFanDuty(int fan, float pwm) {
  if (fan < 0 || fan > MAX_FANS) {
    return ERR_SETFAN_BAD;
  }

  // if a pin factor is present, use it instead
  if (UpdateFanPinning(fan)) {
    return ERR_SETFAN_PINNED;
  }

  FanCurrentDutyCycle[fan] = clamp(fan, pwm);
  return ERR_SETFAN_OKAY;
}

// Syncs the actual fan speed to some fraction of the system's requested speed.
bool UpdateFanPinning(int fan) {
  if (
    FanSystemPinningFactors[fan] > 0 &&
    FanSystemDutyCycle[fan] > 0
  ) {
    FanCurrentDutyCycle[fan] = clamp(fan, (int)(FanSystemDutyCycle[fan] * FanSystemPinningFactors[fan]));
    return true;
  } else {
    return false;
  }
}

// Specify that the given fan should linearly track the system input speed, but
// multiplied by the given factor.
int PinFan(int fan, float factor) {
  if (fan < 0 || fan > MAX_FANS) {
    return ERR_SETFAN_BAD;
  } else if (factor < 0) {
    factor = 0;
  } else if (factor > 1) {
    factor = 1;
  }

  FanSystemPinningFactors[fan] = factor;
  UpdateFanPinning(fan);

  return ERR_SETFAN_OKAY;
}

// Specify that the given fan should be manually controlled.  This will reset
// the fan to the default speed.
int UnpinFan(int fan) {
  if (fan < 0 || fan > MAX_FANS) {
    return ERR_SETFAN_BAD;
  }

  FanSystemPinningFactors[fan] = 0;
  FanCurrentDutyCycle[fan] = clamp(fan, INIT_FAN_SPEED);

  UpdateFanPinning(fan);

  return ERR_SETFAN_OKAY;
}

void UpdatePWM() {
  for(int i = 0; i < MAX_FANS; i++) {
    UpdateFanPinning(i);

    // subtract from max pwm to produce an inverted PWM
    // (I'm gonna call this a "suppression wave")
    analogWrite(FanOutputPins[i], (int)(PWM_UPPER) - FanCurrentDutyCycle[i]);
  }
}

void setup() {
  TCCR0B = _BV(CS00);
  TCCR1B = _BV(CS00);
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);

  // Set initial fan states
  for(int i = 0; i < MAX_FANS; i++) {
    // set default initial bounds
    FanBounds[i][0] = DEFAULT_FAN_MIN_DC;
    FanBounds[i][1] = DEFAULT_FAN_MAX_DC;

    pinMode(FanOutputPins[i], OUTPUT);
    SetFanDuty(i, INIT_FAN_SPEED);

    // initialize the "what the motherboard wants us to do" array
    FanSystemDutyCycle[i] = 0;

    // initialize "pinning" factor, which (if > 0) specifies that a given fan should
    // multiply whatever the system wants the speed to be by the pinning factor
    FanSystemPinningFactors[i] = 0;
  }

  // Setup pins
  pinMode(LED_PIN, OUTPUT); // green BOOT LED
  digitalWrite(LED_PIN, HIGH);

  // update time
  // Uptime = millis();

  Serial.begin(SERIAL_BAUD);
  CliInit();
}

void loop() {
  // this is used to simulate the system input PWM changing by having it
  // move up and down over time.
  // debugSystemSimIterate();

  // update time
  // Uptime = millis();

  UpdatePWM();
  digitalWrite(LED_PIN, HIGH);

  if (CliReadLine(CliLine)) {
    int res = CliExec(CliLine);

    if (res >= 0) {
      Serial.println("OK");
    } else {
      Serial.println("ERR");
    }

    CliReset();
  }

  // if (MonitorMode && (Uptime - LastMonitorEmit) > MONITOR_INTV_MS) {
  //   for(int i = 0; i < MAX_FANS; i++) {
  //     writeFan(i);
  //   }

  //   LastMonitorEmit = Uptime;
  // }

  Serial.flush();
}