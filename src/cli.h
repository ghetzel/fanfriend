#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"

#define CR     '\r'
#define LF     '\n'
#define BS     '\b'
#define NUL    '\0'
#define SPACE  ' '

char CliLine[CLI_MAX_LINE + 1];
char CliArgs[MAX_NUM_ARGS][ARG_BUF_SIZE];

// Command functions declarations
int CmdHelp();
int CmdSet();
int CmdGet();

// List of functions pointers corresponding to each command
int (*CliCmdFuncs[])(){
    &CmdHelp,
    &CmdSet,
    &CmdGet
};

// List of command names
const char *CliCmdStrings[] = {
    "help",
    "set",
    "get"
};

// command usage summaries
const char *CliCmdHelp[] = {
    "            Show this help text.",
    " FANID DUTY  Specify the duty cycle for the given fan FANID, or -1 for all fans.",
    " [FANID]     Report on the current state of one or all fans."
};

const int CliNumCommands = sizeof(CliCmdStrings) / sizeof(char *);
const char *CliEolDelimiters = ", \n";

void CliInit() {
  Serial.println("OK " + String(VERSION));
  Serial.print("> ");
}

bool CliReadLine(char *line) {
  static uint8_t charsRead = 0;

  while (Serial.available()) {
    char c = Serial.read();
    Serial.print(c);

    switch (c) {
      case CR:
      case LF:
        line[charsRead] = NUL;

        if (charsRead > 0)  {
          charsRead = 0;
          return true;
        }

        break;
      case BS:
        if (charsRead > 0) {
          line[--charsRead] = NUL;
          Serial << byte(SPACE) << byte(BS);
        }

        break;
      default:
        if (charsRead < CLI_MAX_LINE) {
          line[charsRead++] = c;
        }

        line[charsRead] = NUL;
        break;
    }
  }

  return false;
}

void CmdWarn(char *msg) {
  Serial.println("WARN " + String(msg));
}

void CmdErr(char *msg) {
  Serial.println("ERR " + String(msg));
}

void CliReset() {
  memset(CliLine, 0, CLI_MAX_LINE);
  memset(CliArgs, 0, sizeof(CliArgs[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);
  Serial.print("> ");
}

int CliExec(char *l) {
  String line = String(l);
  line.trim();

  String arg = "";
  int argi = 0;

  for (int i = 0; i < line.length(); i++) {
    if (line[i] == ' ' || line[i] == ',') {
      arg.toLowerCase();
      arg.toCharArray(CliArgs[argi++], ARG_BUF_SIZE);
      arg = "";
    } else {
      arg += line[i];
    }
  }

  if (arg.length()) {
    arg.toLowerCase();
    arg.toCharArray(CliArgs[argi], ARG_BUF_SIZE);
  }

  // for debugging the args parser
  // Serial.println("");
  // for (int j = 0; j < MAX_NUM_ARGS; j++) {
  //   Serial.println("ARG " + String(j, DEC) + ": " + String(CliArgs[j]));
  // }

  for (int i = 0; i < CliNumCommands; i++) {
      if(strcmp(CliArgs[0], CliCmdStrings[i]) == 0){
          return (*CliCmdFuncs[i])();
      }
  }

  CmdErr("Unrecognized command");
  return -1;
}

int CmdHelp() {
  Serial.println("The following commands are available:");

  for (int i = 0; i < CliNumCommands; i++) {
    Serial.print("  ");
    Serial.println(String(CliCmdStrings[i]) + String(CliCmdHelp[i]));
  }
}

int CmdSet() {
  int fan = String(CliArgs[1]).toInt();
  int duty = String(CliArgs[2]).toInt();

  if (duty <= 0 || duty >= 256) {
    duty = INIT_FAN_SPEED;
    CmdWarn("Duty value out of bounds, using default");
  }

  if (fan > 0 && fan <= MAX_FANS) {
    FanCurrentDutyCycle[fan-1] = duty;
    writeFan(fan-1);
  } else if (fan < 0) {
    for(int i = 0; i < MAX_FANS; i++) {
      FanCurrentDutyCycle[i] = duty;
      writeFan(i);
    }
  } else {
    CmdErr("Invalid FAN ID");
    return -1;
  }
}

int CmdGet() {
  if (String(CliArgs[1]).length()) {
    int fan = String(CliArgs[1]).toInt();
    writeFan(fan-1);
  } else {
    for(int i = 0; i < MAX_FANS; i++) {
      writeFan(i);
    }
  }
}
