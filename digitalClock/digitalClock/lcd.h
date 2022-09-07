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

#define LCD_DATA D			//Port PC4-PC7 are connected to D4-D7
#define LCD_DATA_POS 3		//PD3-PD6 are connected to lcd data pins

#define LCD_E D 			//Enable/strobe signal
#define LCD_E_POS	PD2		//Position of enable in above port

#define LCD_RS D			//RS SIGNAL
#define LCD_RS_POS PD0

#define LCD_RW D			//RW SIGNAL
#define LCD_RW_POS PD1

/***********************************************

LCD Type Selection
Uncomment Just one of them

************************************************/

//#define LCD_TYPE_202	//For 20 Chars by 2 lines

//#define LCD_TYPE_204	//For 20 Chars by 4 lines

#define LCD_TYPE_162	//For 16 Chars by 2 lines

//#define LCD_TYPE_164	//For 16 Chars by 4 lines


//************************************************

	


//************************************************

#define LS_BLINK 0B00000001
#define LS_ULINE 0B00000010
#define LS_NONE	 0B00000000



/***************************************************
			F U N C T I O N S
****************************************************/



void LCDInit(uint8_t style);
void LCDWriteString(const char *msg);
void LCDWriteInt(int val,unsigned int field_length);
void LCDWritefloat(double val,unsigned int flbp,unsigned int flap);

	/*
	val : floating data, double type here..
	flbp : field length before point
	flap : field length after point
	*/

void LCDGotoXY(uint8_t x,uint8_t y);

//Low level
/*
Yes you can print the content of a varriable like content in the serial udr register.

1)char type : USE function LCDData(char_varriable);
2)int type see: LCDWriteInt() & LCDWriteIntXY() functions.

*/
void LCDByte(uint8_t,uint8_t);
#define LCDCmd(c) (LCDByte(c,0))
#define LCDData(d) (LCDByte(d,1))

void LCDBusyLoop();





/***************************************************
			F U N C T I O N S     E N D
****************************************************/


/***************************************************
	M A C R O S
***************************************************/
#define LCDClear() LCDCmd(0b00000001)
#define LCDHome() LCDCmd(0b00000010);

#define LCDWriteStringXY(x,y,msg) {\
 LCDGotoXY(x,y);\
 LCDWriteString(msg);\
}

#define LCDWriteIntXY(x,y,val,fl) {\
 LCDGotoXY(x,y);\
 LCDWriteInt(val,fl);\
}
#define LCDWritefloatXY(x,y,val,flbp,flap) {\
LCDGotoXY(x,y);\
LCDWritefloat(val,flbp,flap);\
}
/***************************************************/




/*_________________________________________________________________________________________*/
#endif




