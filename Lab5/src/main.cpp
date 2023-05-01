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
alarmSound ledStatus = ALARMOFF;

int main()
{
  initI2C();
  initPWMTimer3();
  initSPIMaster();
  initSwitchPB7();
  initTimer1();
  //values for taking the current position of the gyroscope
  signed int X_val = 0;
  signed int Y_val = 0;
  signed int Z_val = 0;
  //values for the default state of the gyroscope
  signed int X_val_init = 0;
  signed int Y_val_init = 0;
  signed int Z_val_init = 0;
  //threshold for the 
  signed int threshhold = 6000; 
  boolean isReset = 1;  


  Serial.begin(9600);
  sei(); // activate global interrupts

  StartI2C_Trans(SLA);
  write(PWR_MGMT); // write to power management
  write(WAKEUP); // write the wakeup command
  StopI2C_Trans();
  
  // display setup
  SPI_write_execute(0x0A,0x08); //Brightness control
  SPI_write_execute(0x0B,0x07); //scan all rows and collumns
  SPI_write_execute(0x0C,0x01); //set shutdown register to normal operation
  SPI_write_execute(0x0F,0x00); //display test register set to normal operation
  
  
    //reading inital values
    delayMs(1);
    Read_from(SLA,SL_MEMA_XAX_HIGH);

    X_val_init = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_XAX_LOW);
    X_val_init = (X_val_init<<8)|Read_data();

    Read_from(SLA,SL_MEMA_YAX_HIGH);
    Y_val_init = Read_data();              //read upper value
    Read_from(SLA,SL_MEMA_YAX_LOW);
    Y_val_init = (Y_val_init<<8)|Read_data();    //read lower value
    
    Read_from(SLA,SL_MEMA_ZAX_HIGH);
    Z_val_init = Read_data();              //read upper value
    Read_from(SLA,SL_MEMA_ZAX_LOW);
    Z_val_init = (Z_val_init<<8)|Read_data();    //read lower value    
    
    StopI2C_Trans();
  
  
  
  int freq = 5000;
  
  while (1)
  {
    //Reading x data
    delayMs(100);
    Read_from(SLA,SL_MEMA_XAX_HIGH);

    X_val = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_XAX_LOW);
    X_val = (X_val<<8)|Read_data();    //read lower value
    //StopI2C_Trans();
    Serial.print("Xinit = ");
    Serial.println(X_val_init);
    Serial.print("X = ");
    Serial.println(X_val);

    //Reading y data
    
    Read_from(SLA,SL_MEMA_YAX_HIGH);

    Y_val = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_YAX_LOW);
    Y_val = (Y_val<<8)|Read_data();    //read lower value
    
    Serial.print("Yinit = ");
    Serial.println(Y_val_init);
    //Serial.print("Y threshold max = ");
    //Serial.println(threshhold_val_y_max);
    //Serial.print("Y threshold min = ");
    //Serial.println(threshhold_val_y_min);
    Serial.print("Y = ");
    Serial.println(Y_val);
    
    //StopI2C_Trans();

    //Reading z data
    
    Read_from(SLA,SL_MEMA_ZAX_HIGH);

    Z_val = Read_data();              //read upper value

    Read_from(SLA,SL_MEMA_ZAX_LOW);
    Z_val = (Z_val<<8)|Read_data();    //read lower value
    

    Serial.print("Zinit = ");
    Serial.println(Z_val_init);
    
    //Serial.print("Z max threshold = ");
    //Serial.println(threshhold_val_z_max);
    //Serial.print("Z min threshold = ");
    //Serial.println(threshhold_val_z_min);
    Serial.print("Z = ");
    Serial.println(Z_val);
    Serial.println("-----");
    StopI2C_Trans();
    if ((abs(X_val - abs(-1300)) > threshhold) || (abs(Y_val - abs(-250)) > threshhold) || (abs(Z_val - abs(20696)) > threshhold)){
      if(!isReset){
        isReset = 1;
        alarmStatus = ALARMON;
      }
      ledStatus = ALARMON;
    }
    else{
      isReset = 0;
      ledStatus = ALARMOFF;
    }
    

    if (ledStatus == ALARMON)
    {
      SPI_write_execute(0x01,0b00000000);
      SPI_write_execute(0x02,0b00000000);
      SPI_write_execute(0x03,0b00100100);
      SPI_write_execute(0x04,0b00000000);
      SPI_write_execute(0x05,0b00011000);
      SPI_write_execute(0x06,0b01100110);
      SPI_write_execute(0x07,0b10000001);
      SPI_write_execute(0x08,0b00000000);
    }
    else
    {
       SPI_write_execute(0x01,0b00000000);
      SPI_write_execute(0x02,0b00000000);
      SPI_write_execute(0x03,0b00100100);
      SPI_write_execute(0x04,0b00000000);
      SPI_write_execute(0x05,0b10000001);
      SPI_write_execute(0x06,0b01100110);
      SPI_write_execute(0x07,0b00111100);
      SPI_write_execute(0x08,0b00000000);
    }

      if (alarmStatus == ALARMON)
      {
      change_frequency(freq);
      if(freq == 5000){
        freq = 1000;
      }
      else{
        freq = 5000;
      }
      //Serial.println("on");
      }
      else
      {
        change_frequency(0);
       // Serial.println("off");
      
      }
      
    }

  return 0;
}

ISR(INT0_vect){
  
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
}
else if(state == DEBOUNCE_RELEASE){
  delayMs(1);
debouncing = false;
}

}
}
