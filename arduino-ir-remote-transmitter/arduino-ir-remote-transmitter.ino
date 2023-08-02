/*
  Arduino IR remote transmitter

  By IOElectro
  1402/05/11

  carrier frequency : 37.7 KHz
  
  Key             [D2,D4,D5]  Pin 2,4,5 internal pullup ]==-------<KEY>----------------[GND
  Output-signal   [D3]        Pin 3                     ]==-------<IR-TX>---<R-330>----[+5V
  Led             [B5]        Pin 13                    ]==-------<LED>-----<R-330>----[GND

  https://ioelectro.ir

*/

#include <stdio.h>
#include <stdint.h>

// Min tx ON or OFF puls width 
#define TIME_SLOT_US 580

// Config & start timer 2 for generate 37.7 KHz PWM 50% DC
void pwm_start()
{
  // Timer/Counter 2 initialization
  // Clock source: System Clock
  // Clock value: 2000/000 kHz
  // Mode: Fast PWM top=OCR2A
  // OC2A output: Disconnected
  // OC2B output: Non-Inverted PWM
  // Timer Period: 0/0265 ms
  // Output Pulse(s):
  // OC2B Period: 0/0265 ms Width: 0/01325 ms
  ASSR=(0<<EXCLK) | (0<<AS2);
  TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (1<<WGM20);
  TCCR2B=(1<<WGM22) | (0<<CS22) | (1<<CS21) | (0<<CS20);
  TCNT2=0x00;
  OCR2A=0x34;
  OCR2B=0x1A;
}

// Stop Timer2 PWM 
void pwm_stop()
{
  TCCR2A=0x00;
  TCCR2B=0x00;
}

// Set LED to Low & Start genetate PWM
void set_pwm()
{
  PORTB=PORTB&(~(1<<5));
  PORTD=PORTD&(~(1<<3));
  pwm_start();
}

// Stop PWM and Set output and LED to Hi as IDLE
void reset_pwm()
{
  pwm_stop();
  PORTD=PORTD|(1<<3);
  PORTB=PORTB|(1<<5);
}

// Sending Data using PWM
void signal(char * s)
{
  int i,j=strlen(s);
  for(i=0;i<j;i++)
  {
    if(s[i]=='1')
    {
      reset_pwm();
   }
   else if(s[i]=='0')
   {
    set_pwm();
   }
   else continue;
   delayMicroseconds(TIME_SLOT_US);
  }
  reset_pwm();
}

// StartUp init
void setup()
{
  DDRB=(1<<5); // LED pin dir

  DDRD=(1<<3); // PWM & Keys pin dir
  PORTD=0xff;  // Enable PullUp

  reset_pwm();
}

// Loooooop!!
void loop()
{  
  // Key Pressed
  if(digitalRead(2)==0)
  {
        signal("00000000000000011111110111010101010101010101110111011101110111011101110101110101110111010101011101011101010111011101110");
        delay(40);
        while(digitalRead(2)==0)
        {
          signal("00000000000000011110");
          delay(100);
        }
        
  }
  else if(digitalRead(4)==0)
  {
        signal("00000000000000011111110111010101010101010101110111011101110111011101110111010101110111010101010111011101010111011101110");
        delay(40);
        while(digitalRead(4)==0)
        {
          signal("00000000000000011110");
          delay(100);
        }
  }
  else if(digitalRead(5)==0)
  {
        signal("00000000000000011111110111010101010101010101110111011101110111011101110111011101011101110101010101011101010111011101110");
        delay(40);
        while(digitalRead(5)==0)
        {
          signal("00000000000000011110");
          delay(100);
        }
  }
  delay(200);
}
