/*
 * LCD_ADC_DEMO.c
 *
 * Created: 23-Sep-17 3:44:05 PM
 * Author : Sayeed
 */ 

#include <avr/io.h>
#include "lcd.h"
#include <util/delay.h>
#include <math.h>
#define sample 174 // 345//174

void adcinit(void);
int ADC_Read(void);
int samples[sample],i;
float sqsum, analog_volt;

int main(void)
{
    LCD1Init(LS_NONE);
	LCD1WriteStringXY(0,0,"                ");
	LCD1WriteStringXY(0,1,"                ");
	LCD1WriteStringXY(0,0,"Voltage Reading:");
	adcinit();
	//int num=0;
	while (1) 
    {
		for(i = 0; i<sample; i++)
		{
			samples[i] = ADC_Read();
		}
		
		for(i = 0; i<sample; i++)
		{
			samples[i]-=512;
			analog_volt = 88.47 * samples[i] / 1023.0 ;
			sqsum += (analog_volt*analog_volt);
		}
		
		sqsum = sqrt(sqsum/sample);
		
		LCD1WritefloatXY(0,1,sqsum,2,1);
		LCD1WriteStringXY(6,1,"Volts");

    }
}

void adcinit()
{
	ADCSRA |= 1<<ADEN; // Enable pin high
	//Prescalar value 4, ADPS2 = 0, ADPS1 = 1, ADPS0 = 0 ;
	ADCSRA |= (1<<ADPS1) ;
	ADMUX |= 1 << REFS0; // Now we are using AVCC voltage, internally 5 volts.
}

int ADC_Read()
{
	int adc_value = 0;
	ADMUX |= 1<<MUX0; // we are using channel 1 as default
	ADCSRA |= 1<<ADSC; // conversion started
	//wait until flag becomes 1, conversion complete
	while((ADCSRA & (1<<ADIF)));
	ADCSRA |= 1<<ADIF ; // started a new conversion
	adc_value=ADCW;//the value is saved at ADCW register, we are reading it
	return adc_value;
}