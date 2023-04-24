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

    //reading inital values
    delayMs(1);
    Read_from(SLA,SL_MEMA_YAX_HIGH);
    Y_val_init = Read_data();              //read upper value
    Read_from(SLA,SL_MEMA_YAX_LOW);
    Y_val_init = (Y_val_init<<8)|Read_data();    //read lower value
    
    Read_from(SLA,SL_MEMA_ZAX_HIGH);
    Z_val_init = Read_data();              //read upper value
    Read_from(SLA,SL_MEMA_ZAX_LOW);
    Z_val_init = (Z_val_init<<8)|Read_data();    //read lower value    
    
    StopI2C_Trans();
  
  threshhold_val_y_max = Y_val_init+6000;
  threshhold_val_y_min = Y_val_init-6000;
  threshhold_val_z_max = Z_val_init+6000;
  threshhold_val_y_min = Y_val_init-6000;
  
  while (1)
  {
    delayMS(1000);
    //Reading x data
    
    Read_from(SLA,SL_MEMA_XAX_HIGH);

    X_val = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_XAX_LOW);
    X_val = (X_val<<8)|Read_data();    //read lower value

    Serial.print("X = ");
    Serial.println(X_val);

    //Reading y data
    
    Read_from(SLA,SL_MEMA_YAX_HIGH);

    Y_val = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_YAX_LOW);
    Y_val = (Y_val<<8)|Read_data();    //read lower value
    Serial.print("Yinit = ");
    Serial.println(Y_val_init);
    Serial.print("Y threshold max = ");
    Serial.println(threshhold_val_y_max);
    Serial.print("Y threshold min = ");
    Serial.println(threshhold_val_y_min);
    Serial.print("Y = ");
    Serial.println(Y_val);

    //Reading z data
    
    Read_from(SLA,SL_MEMA_ZAX_HIGH);

    Z_val = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_ZAX_LOW);
    Z_val = (Z_val<<8)|Read_data();    //read lower value

    Serial.print("Zinit = ");
    Serial.println(Z_val_init);
    
    Serial.print("Z max threshold = ");
    Serial.println(threshhold_val_z_max);
    Serial.print("Z min threshold = ");
    Serial.println(threshhold_val_z_min);
    Serial.print("Z = ");
    Serial.println(Z_val);
    
    StopI2C_Trans();
    
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
