#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#include "lcd1.h"

//Custom Charset support
//#include "custom_char.h"

#define LCD1_DATA_PORT 	PORT(LCD1_DATA)
#define LCD1_E_PORT 	PORT(LCD1_E)
#define LCD1_RS_PORT 	PORT(LCD1_RS)
#define LCD1_RW_PORT 	PORT(LCD1_RW)

#define LCD1_DATA_DDR 	DDR(LCD1_DATA)
#define LCD1_E_DDR 		DDR(LCD1_E)
#define LCD1_RS_DDR 	DDR(LCD1_RS)
#define LCD1_RW_DDR 	DDR(LCD1_RW)

#define LCD1_DATA_PIN	PIN(LCD1_DATA)

#define SET1_E() (LCD1_E_PORT|=(1<<LCD1_E_POS))
#define SET1_RS() (LCD1_RS_PORT|=(1<<LCD1_RS_POS))
#define SET1_RW() (LCD1_RW_PORT|=(1<<LCD1_RW_POS))

#define CLEAR1_E() (LCD1_E_PORT&=(~(1<<LCD1_E_POS)))
#define CLEAR1_RS() (LCD1_RS_PORT&=(~(1<<LCD1_RS_POS)))
#define CLEAR1_RW() (LCD1_RW_PORT&=(~(1<<LCD1_RW_POS)))

#ifdef LCD1_TYPE_162
	#define LCD1_TYPE_204
#endif

#ifdef LCD1_TYPE_202
	#define LCD1_TYPE_204
#endif


void LCD1Byte(uint8_t c,uint8_t isdata)
{
//Sends a byte to the LCD in 4bit mode
//cmd=0 for data
//cmd=1 for command


//NOTE: THIS FUNCTION RETURS ONLY WHEN LCD HAS PROCESSED THE COMMAND

uint8_t hn,ln;			//Nibbles
uint8_t temp;

hn=c>>4;
ln=(c & 0x0F);

if(isdata==0)
	CLEAR1_RS();
else
	SET1_RS();

_delay_us(0.500);		//tAS

SET1_E();

//Send high nibble

temp=(LCD1_DATA_PORT & (~(0X0F<<LCD1_DATA_POS)))|((hn<<LCD1_DATA_POS));
LCD1_DATA_PORT=temp;

_delay_us(1);			//tEH

//Now data lines are stable pull E low for transmission

CLEAR1_E();

_delay_us(1);

//Send the lower nibble
SET1_E();

temp=(LCD1_DATA_PORT & (~(0X0F<<LCD1_DATA_POS)))|((ln<<LCD1_DATA_POS));

LCD1_DATA_PORT=temp;

_delay_us(1);			//tEH

//SEND

CLEAR1_E();

_delay_us(1);			//tEL

LCD1BusyLoop();
}

void LCD1BusyLoop()
{
	//This function waits till lcd is BUSY

	uint8_t busy,status=0x00,temp;

	//Change Port to input type because we are reading data
	LCD1_DATA_DDR&=(~(0x0f<<LCD1_DATA_POS));

	//change LCD mode
	SET1_RW();		//Read mode
	CLEAR1_RS();		//Read status

	//Let the RW/RS lines stabilize

	_delay_us(0.5);		//tAS


	do
	{

		SET1_E();

		//Wait tDA for data to become available
		_delay_us(0.5);

		status=(LCD1_DATA_PIN>>LCD1_DATA_POS);
		status=status<<4;

		_delay_us(0.5);

		//Pull E low
		CLEAR1_E();
		_delay_us(1);	//tEL

		SET1_E();
		_delay_us(0.5);

		temp=(LCD1_DATA_PIN>>LCD1_DATA_POS);
		temp&=0x0F;

		status=status|temp;

		busy=status & 0b10000000;

		_delay_us(0.5);
		CLEAR1_E();
		_delay_us(1);	//tEL
	}while(busy);

	CLEAR1_RW();		//write mode
	//Change Port to output
	LCD1_DATA_DDR|=(0x0F<<LCD1_DATA_POS);

}

void LCD1Init(uint8_t style)
{
	/*****************************************************************

	This function Initializes the lcd module
	must be called before calling lcd related functions

	Arguments:
	style = LS_BLINK,LS_ULINE(can be "OR"ed for combination)
	LS_BLINK :The cursor is blinking type
	LS_ULINE :Cursor is "underline" type else "block" type

	*****************************************************************/

	//After power on Wait for LCD to Initialize
	_delay_ms(30);

	//Set IO Ports
	LCD1_DATA_DDR|=(0x0F<<LCD1_DATA_POS);
	LCD1_E_DDR|=(1<<LCD1_E_POS);
	LCD1_RS_DDR|=(1<<LCD1_RS_POS);
	LCD1_RW_DDR|=(1<<LCD1_RW_POS);

	LCD1_DATA_PORT&=(~(0x0F<<LCD1_DATA_POS));
	CLEAR1_E();
	CLEAR1_RW();
	CLEAR1_RS();

	//Set 4-bit mode
	_delay_us(0.3);	//tAS

	SET1_E();
	LCD1_DATA_PORT|=((0b00000010)<<LCD1_DATA_POS); //[B] To transfer 0b00100000 i was using LCD_DATA_PORT|=0b00100000
	_delay_us(1);
	CLEAR1_E();
	_delay_us(1);

	//Wait for LCD to execute the Functionset Command
	LCD1BusyLoop();                                    //[B] Forgot this delay

	//Now the LCD is in 4-bit mode

	LCD1Cmd(0b00001100|style);	//Display On
	LCD1Cmd(0b00101000);			//function set 4-bit,2 line 5x7 dot format

	/* Custom Char */
	LCD1Cmd(0b01000000);

	uint8_t __i;
	//for(__i=0;__i<sizeof(__cgram);__i++)
		//LCDData(__cgram[__i]);

	LCD1GotoXY(0,0);

}
void LCD1WriteString(const char *msg)
{
	/*****************************************************************

	This function Writes a given string to lcd at the current cursor
	location.

	Arguments:
	msg: a null terminated string to print

	Their are 8 custom char in the LCD they can be defined using
	"LCD Custom Character Builder" PC Software.

	You can print custom character using the % symbol. For example
	to print custom char number 0 (which is a degree symbol), you
	need to write

		LCDWriteString("Temp is 30%0C");
		                          ^^
								  |----> %0 will be replaced by
								  		custom char 0.

	So it will be printed like.

		Temp is 30°C

	In the same way you can insert any syblom numbered 0-7


	*****************************************************************/
 while(*msg!='\0')
 {
 	//Custom Char Support
	if(*msg=='%')
	{
		msg++;
		int8_t cc=*msg-'0';

		if(cc>=0 && cc<=7)
		{
			LCD1Data(cc);
		}
		else
		{
			LCD1Data('%');
			LCD1Data(*msg);
		}
	}
	else
	{
		LCD1Data(*msg);
	}
	msg++;
 }
}

void LCD1WriteInt(int val,unsigned int field_length)
{
	/***************************************************************
	This function writes a integer type value to LCD module

	Arguments:
	1)int val	: Value to print

	2)unsigned int field_length :total length of field in which the value is printed
	must be between 1-5 if it is -1 the field length is no of digits in the val

	****************************************************************/

	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(val)
	{
	str[i]=val%10;
	val=val/10;
	i--;
	}
	if(field_length==-1)
		while(str[j]==0) j++;
	else
		j=5-field_length;

	if(val<0) LCD1Data('-');
	for(i=j;i<5;i++)
	{
	LCD1Data(48+str[i]);
	}
}

//float number printing function

void LCD1Writefloat(double val,unsigned int flbp,unsigned int flap)

{
	/*
	val : floating data, double type here..
	flbp : field length before point
	flap : field length after point
	*/

	uint16_t y=0;
	uint8_t x;
	double f=0.0;

	f = val;
	y = f;
	f -= y;
	for(x=0;x<flap;x++)
	f *=10;

	LCD1WriteInt(y,flbp);
	LCD1WriteString(".");
	y = f;
	LCD1WriteInt(y,flap);

}

void LCD1GotoXY(uint8_t x,uint8_t y)
{
if(x<40)
{
switch(y)
{
case 0://1 line starts at 0×80(0b10000000)
x|=0b10000000;
break;
case 1://2 line starts at 0xC0(0b11000000)
x|=0b11000000;
break;
case 2://3 line starts at 0×94(0b10010100)
x|=0b10010100;
break;
case 3://4 line starts at 0xD4(0b11010100)
x|=0b11010100;
break;
}
LCD1Cmd(x);
}
}
/*
void LCDGotoXY(uint8_t x,uint8_t y)
{
 	if(x>=20) return;

	#ifdef LCD_TYPE_204

	switch(y)
	{
		case 0:
			break;
		case 1:
			x|=0b01000000;
			break;
		case 2:
			x+=0x14;
			break;
		case 3:
			x+=0x54;
			break;
	}

	#endif

	#ifdef LCD_TYPE_164
	switch(y)
	{
		case 0:
			break;
		case 1:
			x|=0b01000000;
			break;
		case 2:
			x+=0x10;
			break;
		case 3:
			x+=0x50;
			break;
	}

	#endif

	x|=0b10000000;
  	LCDCmd(x);
}



*/

