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
    /*
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
    // combine and serial print information
    int x = SL_MEMA_XAX_HIGH << 8 | SL_MEMA_XAX_LOW;
    int y = SL_MEMA_YAX_HIGH << 8 | SL_MEMA_YAX_LOW;
    int z = SL_MEMA_ZAX_HIGH << 8 | SL_MEMA_ZAX_LOW;
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
    */
    // Read from X high register -> Read from X low register
    Read_from(SLA, SL_MEMA_XAX_HIGH);
    int x = Read_data();
    Read_from(SLA, SL_MEMA_XAX_LOW);
    int x = (x << 8) | Read_data();
    // Read from Y high register -> Read from Y low register
    Read_from(SLA, SL_MEMA_YAX_HIGH);
    int y = Read_data();
    Read_from(SLA, SL_MEMA_YAX_LOW);
    int y = (y << 8) | Read_data();
    // Read from Z high register -> Read from Z low register
    Read_from(SLA, SL_MEMA_ZAX_HIGH);
    int z = Read_data();
    Read_from(SLA, SL_MEMA_ZAX_LOW);
    int z = (z << 8) | Read_data();

    float xAng = map(x, 265, 402, -90, 90);
    float yAng = map(y, 265, 402, -90, 90);
    float zAng = map(z, 265, 402, -90, 90);
    float xRot = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
    float yRot = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
    float zRot = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

    // combine and serial print information
    //float x = SL_MEMA_XAX_HIGH << 8 | SL_MEMA_XAX_LOW;
    //float y = SL_MEMA_YAX_HIGH << 8 | SL_MEMA_YAX_LOW;
    //float z = SL_MEMA_ZAX_HIGH << 8 | SL_MEMA_ZAX_LOW;
    Serial.println(xRot);
    Serial.println(yRot);
    Serial.println(zRot);
    delay(400);
    
    if (true) // change to whatever our threshold is
    {
      if (alarmStatus == ALARMOFF)
      {
        //change_frequency(400);
        alarmStatus = ALARMON;
      }
      else
      {
        turn_off();
        alarmStatus = ALARMOFF;
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
