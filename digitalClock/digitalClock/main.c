/*
 * digitalClock.c
 *
 * Created: 14-Oct-17 4:06:16 PM
 * Author : Sayeed
 */ 
# define F_CPU 1000000UL

#include <avr/io.h>
#include "lcd.h"
#include "lcd1.h"
#include <avr/interrupt.h>

void init_lcd();
void init_lcd1();
void init_clock();

int Second = 0, millis=0, minute = 0, hour = 0;

int main(void)
{
	init_lcd();
	init_lcd1();
	init_clock();
    while (1) 
    {
		
    }
}

void init_clock()//same code of pwm example
{
	TCCR1B |= 1<<CS11 ; //Prescalar 8
	ICR1 = 124 ; // Top value for 1khz PWM , ICR == Input compare register
	TCCR1B |= (1<<WGM13) | (1<<WGM12) ;//fast pwm
	TCCR1A |= (1<<WGM11) ;//fast pwm
	
	sei(); //global interrupt call
	TIMSK1 |= 1<<TOIE1;//Timer/counter interrupt enabled
}

void init_lcd()
{
	LCDInit(LS_NONE);
	LCDClear();
	LCDWriteStringXY(0,0,"Digital Clock");
	LCDWriteStringXY(0,1,"00:00:00");
}

void init_lcd1()
{
	LCD1Init(LS_NONE);
	LCD1Clear();
	LCD1WriteStringXY(0,0,"Welcome");
}

ISR(TIMER1_OVF_vect)
{
	millis++;
	if(millis>=1000)
	{
		Second++;
		millis = 0;
		if(Second >= 60)
		{
			Second = 0 ;
			minute ++ ;
			LCDWriteIntXY(6,1,Second,2);
			if(minute >= 60)
			{
				minute = 0;
				hour ++;
				LCDWriteIntXY(0,1,hour,2);
				LCDWriteIntXY(3,1,minute,2);
			}
			else
			LCDWriteIntXY(3,1,minute,2);
		}
		else
		LCDWriteIntXY(6,1,Second,2);
	}
}