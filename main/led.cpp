#include "led.h"
#include <Arduino.h>

void init_led(){
  pinMode(4, OUTPUT); 
  digitalWrite(4, LOW); 

  pinMode(33, OUTPUT); 
  digitalWrite(33, HIGH); 

  turn_on_led_esp();
  turn_on_flash();
  delay(2000);
  turn_off_led_esp();
  turn_off_flash();
}

void turn_on_led_esp(){
  digitalWrite(33, LOW);
}

void turn_off_led_esp(){
  digitalWrite(33, HIGH);
}

void turn_on_flash(){
  digitalWrite(4, HIGH);
}

void turn_off_flash(){
  digitalWrite(4, LOW);
}
