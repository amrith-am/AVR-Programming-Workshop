
#include <avr/io.h>
#include <util/delay.h>
#define baudrate 12 // can be bigger values
void usart_init()
{
	UCSRB |= (1<<TXEN) | (1<<RXEN) ;//page 212
	UBRRL = baudrate ; //page 217 // lower eight bits
	UBRRH = baudrate >> 8 ; // higher eight bits
	UCSRC |= 1 << URSEL ; // enable
	UCSRC |= (1 << UCSZ1) | (1 << UCSZ0); // page 214-215 // data size selection
}

void usart_send(char data)
{
	while(!(UCSRA & (1 << UDRE))); // page 210, wait till data buffer is empty, udre is flag
	UDR = data ;
}

void usart_send_string(char *text)
{
	while(*text != '\0')
	{
		usart_send(*text);
		text ++ ;
	}
}

int main(void)
{
	usart_init();//always sends ascii value
	while(1)
	{
		usart_send('A');
		usart_send('B');
		usart_send('C');
		usart_send_string("Hello there. The temperature today is");
		usart_send_string("\t");
		usart_send_string("38 degree");
		usart_send_string("\n");
		_delay_ms(1000);
	}
}
