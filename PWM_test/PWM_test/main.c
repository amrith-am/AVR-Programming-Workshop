/*
 * PWM_test.c
 *
 * Created: 13-Oct-17 6:24:13 PM
 * Author : Sayeed
 */ 

#include <avr/io.h>


int main(void)
{
    DDRB |= (1<<PB1) | (1<<PB2) ; // OC1A and OC1B are set as output mode
	TCCR1B |= 1<<CS11 ; //Prescalar 8
	ICR1 = 124 ; // Top value for 1khz PWM , ICR == Input compare register
	TCCR1B |= (1<<WGM13) | (1<<WGM12) ;//fast pwm
	TCCR1A |= (1<<WGM11) ;//fast pwm
	OCR1A = 20 ;
	OCR1B = 90 ;
	TCCR1A |= 1<<COM1A1 ;//clear on compare match
	
	 while (1) 
    {
    }
}

