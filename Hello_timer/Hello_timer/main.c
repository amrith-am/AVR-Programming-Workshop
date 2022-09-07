/*
 * Hello_timer.c
 *
 * Created: 07-Oct-17 3:54:34 PM
 * Author : Sayeed
 */ 

#include <avr/io.h>

int overflow = 0;
int main(void)
{
	TCCR0B |= (1<<CS00); // prescalar 1
	TCNT0 = 0; //Timer starts counting
	DDRB |= 1<<PB0;
    while (1) 
    {
		if(TCNT0 >= 250)
		{
			TCNT0 = 0 ;
			overflow ++ ;	
		}
		if(overflow>=3920)
		{
			PORTB ^= 1<<PB0 ;
			overflow = 0;
		}
    }
}

