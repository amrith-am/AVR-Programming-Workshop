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
int samples[sample],i,res,choice;
float sqsum, analog_volt, set_res, resistance;
void LCDCls(void);

int main(void)
{
	choice = 1;
    LCDInit(LS_NONE);
	LCDCls();
	LCDWriteStringXY(0,0,"<   Voltage    >");
	adcinit();
	DDRB = 0 ;
	while(1)
	{
		int inA = PINB & (1<<PB1);
		int inC = PINB & (1<<PB0);
		if(inA)
		{
			if(choice == 1)
			{
				choice = 2;
				LCDWriteStringXY(0,0,"<   Resistor   >");
			}
			else
			{
				choice = 1;
				LCDWriteStringXY(0,0,"<   Voltage    >");
			}
			_delay_ms(500);
		}
		if(inC)
		{
			LCDCls();
			_delay_ms(500);
			if(choice == 1)
			{
				LCDWriteStringXY(0,0,"Volatge : ");
				while (1)
				{
					for(i = 0; i<sample; i++)
					{
						samples[i] = ADC_Read_0();
					}
					
					for(i = 0; i<sample; i++)
					{
						samples[i]-=512;
						analog_volt = 88.47 * samples[i] / 1023.0 ;
						sqsum += (analog_volt*analog_volt);
					}
					
					sqsum = sqrt(sqsum/sample);
					
					LCDWritefloatXY(0,1,sqsum,2,1);
					LCDWriteStringXY(6,1,"Volts");
				}
			}
			else
			{
				LCDWriteStringXY(0,0,"Resistance : ");
				while(1)
				{
					int R1 = PINB & (1<<PB2);
					int R2 = PINB & (1<<PB3);
					int R3 = PINB & (1<<PB4);
					int R4 = PINB & (1<<PB5);

					if(R1)
						set_res = 100 ;
					else if(R2)
						set_res = 10 ;
					else if(R3)
						set_res = 1000 ;
					else if(R4)
						set_res = 100 ;
					else
					{
						LCDWriteStringXY(0,1,"1               ");
						continue;
					}
					
					res  = ADC_Read_1();
					
					if(res!=1023)
					{
						if(R1 || R2)
						{
							resistance = (res*set_res)/(1023.0-res);
							LCDWritefloatXY(0,1,resistance,4,2);
							LCDWriteStringXY(0,7,"K Ohms");
						}
						else
						{
							resistance = (res*set_res)/(1023.0-res);
							LCDWritefloatXY(0,1,resistance,4,2);
							LCDWriteStringXY(0,7,"  Ohms");
						}
					}
					else
					{
						LCDWriteStringXY(0,1,"1               ");
					}
					
				}
			}
		}
	}
}

void LCDCls(void)
{
		LCDWriteStringXY(0,0,"                ");
		LCDWriteStringXY(0,1,"                ");
}
void adcinit()
{
	ADCSRA |= 1<<ADEN; // Enable pin high
	//Prescalar value 4, ADPS2 = 0, ADPS1 = 1, ADPS0 = 0 ;
	ADCSRA |= (1<<ADPS1) ;
	ADMUX |= 1 << REFS0; // Now we are using AVCC voltage, internally 5 volts.
}

int ADC_Read_0()
{
	int adc_value = 0;
	//ADMUX |= 1<<MUX0; // we are using channel 1 as default
	ADCSRA |= 1<<ADSC; // conversion started
	//wait until flag becomes 1, conversion complete
	while((ADCSRA & (1<<ADIF)));
	ADCSRA |= 1<<ADIF ; // started a new conversion
	adc_value=ADCW;//the value is saved at ADCW register, we are reading it
	return adc_value;
}

int ADC_Read_1()
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