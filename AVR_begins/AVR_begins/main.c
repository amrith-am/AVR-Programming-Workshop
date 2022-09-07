/*
 * AVR_begins.c
 *
 * Created: 16-Sep-17 3:47:51 PM
 * Author : Sayeed
 */ 

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRB |= 1<<PB0 ;
	DDRA &= ~(1<<PA0);	
    while (1) 
    {
		if(!(PINA & 1<<PA0))		//when switch pressed, PA0 is 0
		PORTB |= 1<<PB0 ;
		else
		PORTB &= ~(1<<PB0) ;
		
    }
}

