#include "pir.h"
#include <Arduino.h>

unsigned long lastTrigger = 0;
bool startTimer = false;
int pir_pin;

void init_pir(int pin){
  pir_pin = pin;
	pinMode(pir_pin, INPUT);
}

bool warning(){
  int is_moved = digitalRead(pir_pin);
  if(is_moved && !startTimer){
    startTimer = true;
    lastTrigger = millis();
  }else if(startTimer && (millis() - lastTrigger > (timeSeconds*1000))) {
    startTimer = false;
    if(is_moved)
      return true;
  }
  return false;
}
