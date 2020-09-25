#include <Arduino.h>
#include "pitches.h"

#define SUCCESS_SOUND 1
#define WARNING_SOUND 2
#define WRONG_SOUND 3
#define WAITING_SOUND 4
#define ON_BUZZER LOW
#define OFF_BUZZER HIGH

void init_buzzer(int pin);
void tone(int frequency, int duration);
void noTone();
void announce(int sound);
void beep(int duration);
