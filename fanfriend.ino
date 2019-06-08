#define MAX_FANS          6   // Number of fans to allocate
#define START_DUTY_FACTOR 1.0 // How fast the fans should spin on MCL startup (0.0 <= n <= 1.0)

static float FanDutyFactors[MAX_FANS] = {};

// Change the duty cycle on the numbered fan. Clamps the duty factor
// to [0.0, 1.0] and validates the fan index.  Will return 0 on success
// and n<0 on failure.
int setFanDuty(int fan, float dutyFactor) {
  if (fan < 0 || fan > MAX_FANS) {
    return -1;
  } else if (dutyFactor < 0) {
    dutyFactor = 0;
  } else if (dutyFactor > 1) {
    dutyFactor = 1;
  }

  FanDutyFactors[fan] = dutyFactor;
  return 0;
}

void setup() {
  // Set initial state
  // -----------------------------------------------------------------------------------------------
  for(int i = 0; i < MAX_FANS; i++) {
    FanDutyFactors[i] = START_DUTY_FACTOR;
  }

  // Setup pins
  // -----------------------------------------------------------------------------------------------
  pinMode(7, OUTPUT); // green BOOT LED
}

void loop() {
  for(int i = 0; i < MAX_FANS; i++) {
    for(int j = 0; j < (i+1); j++) {
      digitalWrite(7, HIGH);
      delay(100);
      digitalWrite(7, LOW);
      delay(100);
    }

    delay(2000);

    float factor = FanDutyFactors[i];

    digitalWrite(7, HIGH);
    delay(1000 * factor);
    digitalWrite(7, LOW);
    delay(1000 * factor);
  }
}
