/*

*LCD interfacing
*MUXED 7 segment displays.
*Remote Control
*Serial Communication
*DC Motor Controls with Speed Control
*Analog Measurement for Sensor Interface.
*Temperature Measurement.
*I2C Communication.
*EEPROM Interface
*Real Time Clock (RTC Interface)

The APIs are highly documented and easy to use even by a beginner.

Versions:
	1.0 - 	Basic Lib
	2.0 - 	Added Custom Char Support, Much flexible connection.
			4 line LCD support.

*/

#include <avr/io.h>

#include <util/delay.h>

#include "myutils.h"

#ifndef _LCD_H
#define _LCD_H



/*_________________________________________________________________________________________*/

/************************************************
	LCD CONNECTIONS
*************************************************/

 //MCUCSR|= (1<<JTD);// disable JTAG..
 //MCUCSR|= (1<<JTD);//JTAG interface shares PC2, PC3, PC4 and PC5 of ATmega16. To use these four pins for general I/O operations, JTAG must be disabled.

#define LCD1_DATA D			//Port PC4-PC7 are connected to D4-D7
#define LCD1_DATA_POS 3		//PD3-PD6 are connected to lcd data pins

#define LCD1_E D 			//Enable/strobe signal
#define LCD1_E_POS	PD2		//Position of enable in above port

#define LCD1_RS D			//RS SIGNAL
#define LCD1_RS_POS PD0

#define LCD1_RW D			//RW SIGNAL
#define LCD1_RW_POS PD1

/***********************************************

LCD Type Selection

************************************************/

//#define LCD1_TYPE_202	//For 20 Chars by 2 lines

//#define LCD1_TYPE_204	//For 20 Chars by 4 lines

#define LCD1_TYPE_162	//For 16 Chars by 2 lines

//#define LCD1_TYPE_164	//For 16 Chars by 4 lines


//************************************************




//************************************************

#define LS_BLINK 0B00000001
#define LS_ULINE 0B00000010
#define LS_NONE	 0B00000000



/***************************************************
			F U N C T I O N S
****************************************************/



void LCD1Init(uint8_t style);
void LCD1WriteString(const char *msg);
void LCD1WriteInt(int val,unsigned int field_length);
void LCD1Writefloat(double val,unsigned int flbp,unsigned int flap);

	/*
	val : floating data, double type here..
	flbp : field length before point
	flap : field length after point
	*/

void LCD1GotoXY(uint8_t x,uint8_t y);

//Low level
/*
Yes you can print the content of a varriable like content in the serial udr register.

1)char type : USE function LCDData(char_varriable);
2)int type see: LCDWriteInt() & LCDWriteIntXY() functions.

*/
void LCD1Byte(uint8_t,uint8_t);
#define LCD1Cmd(c) (LCD1Byte(c,0))
#define LCD1Data(d) (LCD1Byte(d,1))

void LCD1BusyLoop();





/***************************************************
			F U N C T I O N S     E N D
****************************************************/


/***************************************************
	M A C R O S
***************************************************/
#define LCD1Clear() LCD1Cmd(0b00000001)
#define LCD1Home() LCD1Cmd(0b00000010);

#define LCD1WriteStringXY(x,y,msg) {\
 LCD1GotoXY(x,y);\
 LCD1WriteString(msg);\
}

#define LCD1WriteIntXY(x,y,val,fl) {\
 LCD1GotoXY(x,y);\
 LCD1WriteInt(val,fl);\
}
#define LCD1WritefloatXY(x,y,val,flbp,flap) {\
LCD1GotoXY(x,y);\
LCD1Writefloat(val,flbp,flap);\
}
/***************************************************/




/*_________________________________________________________________________________________*/
#endif




