/*
 * Hello_interrupt.c
 *
 * Created: 07-Oct-17 4:51:44 PM
 * Author : Sayeed
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
    sei(); // globally enable interrupt
	EIMSK |= 1<<INT0; // Set interrupt pin zero
	EICRA |= 1<<ISC01 ; // Falling edge triggers interrupt
	DDRB |= 1<<PB0;
    while (1) 
    {
		
    }

}

ISR(INT0_vect) // will jump here on interrupt call, we have used INT0 interrupt pin
{
	PORTB ^= (1<<PB0);
}
//ISR(TIMER_OVF)