/*
 * TestProject1.c
 *
 * Created: 17-Mar-17 4:41:21 PM
 * Author : Sayeed
 */ 

#include <avr/io.h>

#include <util/delay.h>

int main()
{
	DDRA |= 1<<PA0 ;		
	while(1)
	{
		
		PORTA |= 1<< PA0 ;	
		_delay_ms(1000); 
		PORTA &= ~(1<<PA0);	
		_delay_ms(250);
	}
	
	return 0;
}