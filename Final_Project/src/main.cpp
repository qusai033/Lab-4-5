#include <avr/io.h>
#include <Arduino.h>
#include "timer.h"
#include "pwm.h"
#include "switch.h"
#include "led.h"
#include "buzzer.h"
#include "PIR.h"
#include "rc522.h"

// states for the switch states
typedef enum states_ {
  WAIT_RELEASE,
  WAIT_PRESS,
  DEBOUNCE_PRESS,
  DEBOUNCE_RELEASE
} state;

typedef enum alarmSound {
  ALARMON,
  ALARMOFF
} alarmSound;

typedef enum servoState {
  LOCKED,
  UNLOCKED
} servoState;

const uint8_t key[] = {0x33, 0xC8, 0xE0, 0xA0}; // Replace with your key

alarmSound alarmStatus = ALARMOFF;
servoState isLocked = UNLOCKED;
int motionCounter = 0;

int main()
{
  Serial.begin(9600);
  initLED();
  initBuzzer();
  initPWMTimer3();
  initSwitchPB4();
  initTimer1();
  initTimer0();
  initPIR();
  initRC522();

  sei(); // activate global interrupts
  while (1)
  {
    uint8_t tag[5];
    bool tagFound = readTag(tag);
    bool tagAuthenticated = false;

    if (tagFound) {
      tagAuthenticated = authenticateTag(tag, key);
      haltTag();
    }

    if(tagAuthenticated){
      isLocked = (isLocked == LOCKED) ? UNLOCKED : LOCKED;
    }

    if(isLocked == LOCKED){
      setLocked(true);
      alternateLED(false);
      Serial.println("Locked");
      if((PINH & ( 1 << PINH4))){
        motionCounter++;
        delayS(1);
        if(motionCounter == 3){
          alarmStatus = ALARMON;
        }
      }
      else{
        motionCounter = 0;
      }
    }
    else{
      setLocked(false);
      alternateLED(true);
      Serial.println("Unlocked");
    }

    if (alarmStatus == ALARMON)
    {
      Serial.println("Alarm on");
      buzzerOn(true);
    }
    else
    {
      Serial.println("Alarm off");
      buzzerOn(false);
    }
  }
  return 0;
}

ISR(PCINT0_vect){
  if(alarmStatus == ALARMON){
    state state = WAIT_PRESS;
    bool debouncing = true;

    while(debouncing){
      if(state == WAIT_PRESS){
        delayMs(1);
        state = DEBOUNCE_PRESS;
      }
      else if(state == DEBOUNCE_PRESS){
        delayMs(1);
        state = WAIT_RELEASE;
      }
      else if(state == WAIT_RELEASE){
        delayMs(1);
        state = DEBOUNCE_RELEASE;
        alarmStatus = ALARMOFF;
        motionCounter = 0;
      }
      else if(state == DEBOUNCE_RELEASE){
        delayMs(1);
        debouncing = false;
      }
    }
  }
  else if(isLocked == UNLOCKED){
    state state = WAIT_PRESS;
    bool debouncing = true;

    while(debouncing){
      if(state == WAIT_PRESS){
        delayMs(1);
        state = DEBOUNCE_PRESS;
      }
      else if(state == DEBOUNCE_PRESS){
        delayMs(1);
        state = WAIT_RELEASE;
      }
      else if(state == WAIT_RELEASE){
        delayMs(1);
        state = DEBOUNCE_RELEASE;
        isLocked = LOCKED;
      }
      else if(state == DEBOUNCE_RELEASE){
        delayMs(1);
        debouncing = false;
      }
    }
  }
}
