#include "buzzer.h"

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int buzzer_pin;
int channel=0;

void init_buzzer(int pin){
	buzzer_pin = pin;
	pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, OFF_BUZZER);
//	beep(500);
//  delay(1000);
//  beep(500);
}

void tone(int frequency, int duration)
{
    ledcRead(channel);
    ledcAttachPin(buzzer_pin, channel);
    ledcWriteTone(channel, frequency);
    if (duration) {
        delay(duration);
        noTone();
    }    
}

void noTone()
{
    ledcDetachPin(buzzer_pin);
    ledcWrite(channel, 0);
}

void beep(int duration){
  digitalWrite(buzzer_pin, ON_BUZZER);
  delay(duration);
  digitalWrite(buzzer_pin, OFF_BUZZER);
}

void announce(int sound){
  switch(sound){
    case SUCCESS_SOUND:
      digitalWrite(buzzer_pin, ON_BUZZER);
      delay(100);
      digitalWrite(buzzer_pin, OFF_BUZZER);
      delay(100);
      digitalWrite(buzzer_pin, ON_BUZZER);
      delay(100);
      digitalWrite(buzzer_pin, OFF_BUZZER);
      break;
     case WRONG_SOUND:
      digitalWrite(buzzer_pin, ON_BUZZER);
      delay(100);
      digitalWrite(buzzer_pin, OFF_BUZZER);
      delay(100);
      digitalWrite(buzzer_pin, ON_BUZZER);
      delay(100);
      digitalWrite(buzzer_pin, OFF_BUZZER);
      delay(100);
      digitalWrite(buzzer_pin, ON_BUZZER);
      delay(100);
      digitalWrite(buzzer_pin, OFF_BUZZER);
      break;
     case WARNING_SOUND:
      for (int thisNote = 0; thisNote < 8; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(melody[thisNote], noteDuration);
        
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone();
      }
      break;  
     case WAITING_SOUND:
      digitalWrite(buzzer_pin, ON_BUZZER);
      delay(1000);
      digitalWrite(buzzer_pin, OFF_BUZZER);
      delay(1000);
      digitalWrite(buzzer_pin, ON_BUZZER);
      delay(1000);
      digitalWrite(buzzer_pin, OFF_BUZZER);
  }
}
