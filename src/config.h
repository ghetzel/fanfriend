#ifndef CONFIG_H_
#define CONFIG_H_

#define VERSION             "fanfriend/0.0.1" // let 'em know who we are
#define USB_VENDOR          0x03EB
#define USB_PRODUCT         0x204F
#define MAX_FANS            6                 // Number of fans to allocate
#define INIT_FAN_SPEED      64                // How fast the fans should spin on MCL startup (0 <= n <= 255)
#define LED_PIN             7
#define CLI_MAX_LINE        256
#define MAX_NUM_ARGS        16
#define ARG_BUF_SIZE        16
#define PWM_UPPER           255
#define SERIAL_BAUD         115200
#define MONITOR_INTV_MS     1000
#define DEFAULT_FAN_MIN_DC  15
#define DEFAULT_FAN_MAX_DC  255

#define ERR_SETFAN_OKAY     0
#define ERR_SETFAN_BAD      -1
#define ERR_SETFAN_PINNED   -2

static unsigned long Uptime = 0;
static unsigned long LastMonitorEmit = 0;
static bool MonitorMode = false;
static int FanCurrentDutyCycle[MAX_FANS] = {};
static int FanBounds[MAX_FANS][2] = {};
static int FanSystemDutyCycle[MAX_FANS] = {};
static float FanSystemPinningFactors[MAX_FANS] = {};

int SetFanDuty(int fan, float range);
int PinFan(int fan, float factor);
int UnpinFan(int fan);

// Clamps a proposed fan PWM duty cycle [0,255] to the configured min/max bounds.
int clamp(int fan, int pwm) {
  int min = FanBounds[fan][0];
  int max = FanBounds[fan][1];

  if (pwm > max) {
    pwm = max;
  } else if (pwm < min) {
    pwm = min;
  }

  return pwm;
}

void writeFan(int i) {
  if (i < 0 || i >= MAX_FANS) {
    return;
  }

  float pinning = FanSystemPinningFactors[i];
  String suffix = "";

  if (pinning) {
    suffix = " PINNED " + String(pinning, 2);
  }

  Serial.println(
    "FAN " + String(i+1, DEC) +
    " ACTUAL " + String(FanCurrentDutyCycle[i], DEC) +
    " SYSTEM " + String(FanSystemDutyCycle[i], DEC) +
    suffix
  );
}

bool debugSysAdd = true;
int x = 0;

void debugSystemSimIterate() {
  if (x++ % 1000 == 0) {
    for(int i = 0; i < MAX_FANS; i++) {
      if (FanSystemDutyCycle[i] >= 255) {
        debugSysAdd = false;
      } else if (FanSystemDutyCycle[i] <= 0) {
        debugSysAdd = true;
      }

      if (debugSysAdd) {
        FanSystemDutyCycle[i] = FanSystemDutyCycle[i] + 1;
      } else {
        FanSystemDutyCycle[i] = FanSystemDutyCycle[i] - 1;
      }
    }
  }
}

#endif // CONFIG_H_