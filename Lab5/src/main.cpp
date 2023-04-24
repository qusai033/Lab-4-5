#include <avr/io.h>
#include <Arduino.h>

#include "i2c.h"
#include "timer.h"
#include "pwm.h"
#include "switch.h"
#include "spi.h"


//addresses for I2C
#define SLA       0x68 //MPU_6050 address with PIN AD0 grounded
#define PWR_MGMT  0x6B
#define WAKEUP    0x00

#define SL_MEMA_XAX_HIGH 0x3B
#define SL_MEMA_XAX_LOW  0x3C

#define SL_MEMA_YAX_HIGH 0x3D
#define SL_MEMA_YAX_LOW  0x3E

#define SL_MEMA_ZAX_HIGH 0x3F
#define SL_MEMA_ZAX_LOW  0x40

#define SL_TEMP_HIGH     0x41
#define SL_TEMP_LOW      0x42

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

alarmSound alarmStatus = ALARMOFF;

int main()
{
  initI2C();
  initPWMTimer3();
  initSPIMaster();
  initSwitchPB7();
  initTimer1();

  Serial.begin(9600);

  sei(); // activate global interrupts

  StartI2C_Trans(SLA);
  write(PWR_MGMT); // write to power management
  write(WAKEUP); // write the wakeup command

  while (1)
  {
    // Read from X high register -> Read from X low register
    Read_from(SLA, SL_MEMA_XAX_HIGH);
    Read_from(SLA, SL_MEMA_XAX_LOW);
    // Read from Y high register -> Read from Y low register
    Read_from(SLA, SL_MEMA_YAX_HIGH);
    Read_from(SLA, SL_MEMA_YAX_LOW);
    // Read from Z high register -> Read from Z low register
    Read_from(SLA, SL_MEMA_ZAX_HIGH);
    Read_from(SLA, SL_MEMA_ZAX_LOW);

    // combine and serial print information
    
    if (true) // change to whatever our threshold is
    {
      if (alarmStatus == ALARMOFF)
      {
        //change_frequency(400);
        alarmStatus = ALARMON;
      }
    }

    StopI2C_Trans();
  }

  return 0;
}

ISR(INT0_vect){
  // only silence if the alarm is on
  if(alarmStatus == ALARMON){
    state state = WAIT_PRESS;
    bool debouncing = true;
    // while we are going through switch press
    while(debouncing){
      if(state == WAIT_PRESS){
        delayMs(1);
        state = DEBOUNCE_PRESS;
      }
      else if(state == DEBOUNCE_PRESS){
        delayMs(1);
        state = WAIT_RELEASE;
      }
      // on release set the alarmStatus to off
      else if(state == WAIT_RELEASE){
        delayMs(1);
        state = DEBOUNCE_RELEASE;
        alarmStatus = ALARMOFF;
      }
      // on release we are no longer pressing switch
      else if(state == DEBOUNCE_RELEASE){
        delayMs(1);
        debouncing = false;
      }
    }
  }
}
