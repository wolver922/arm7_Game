/*********************************************************************************************
* File£º	8led.c
* Author:	embest	
* Desc£º	8-segment digit LED control functions
* History:	
*********************************************************************************************/
#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "iic.h"

/*------------------------------------------------------------------------------------------*/
/*	 								global variables					 				    */
/*------------------------------------------------------------------------------------------*/
int f_nKeyPress;

/*------------------------------------------------------------------------------------------*/
/*	 								function declare						 				    */
/*------------------------------------------------------------------------------------------*/
UINT8T keyboard_test(void);
void keyboard_int(void);
UINT8T key_set(UINT8T ucChar);
/*********************************************************************************************
* name:		keyboard_test
* func:		test 8led
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
UINT8T keyboard_test(void)
{
	int i, j, k;
	UINT8T ucChar, t;
	
//	iic_init();
	
	// set EINT2 interrupt handler
    pISR_EINT2 = (int)keyboard_int;

	//for(;;)
//	{
	    f_nKeyPress = 0;
		rINTMSK = rINTMSK & (~(BIT_GLOBAL|BIT_EINT2));		// enable EINT2 int
		//while(f_nKeyPress == 0);
		iic_read(0x70, 0x1, &ucChar);
		if(ucChar !=0)
		{
			//ucChar = key_set(ucChar);
			return ucChar;
			//if(ucChar < 10) ucChar += 0x30;
			//else if(ucChar < 16) ucChar += 0x37;
			//if(ucChar < 255)
			//	uart_printf("press key %c\n", ucChar);
				//return ucChar;
				
			/*if(ucChar == 0xFF)
			  {
				uart_printf(" press key FUN (exit now)\n\r");
					return;
			  } */
		 }//if(ucChar !=0)
//	}//for(;;)
    //while(1);
}

/*********************************************************************************************
* name:		keyboard_int
* func:		keyboard interrupt handler
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void keyboard_int(void)
{
	UINT8T ucChar;
    //delay(1000);
	rINTMSK = rINTMSK | BIT_EINT2;							// disable EINT2 int
    rI_ISPC = BIT_EINT2;
    f_nKeyPress = 1;
}
/*********************************************************************************************
* name:		key_set
* func:		keyboard setting
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
UINT8T key_set(UINT8T ucChar)
{
	switch(ucChar)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
				ucChar-=1; break;
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
				ucChar-=4; break;
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
				ucChar-=7; break;
		case 25: 
		ucChar = 0xF; 
		break;
		case 26: ucChar = '+'; break;
		case 27: ucChar = '-'; break;
		case 28: ucChar = '*'; break;
		case 29: ucChar = 0xFF; break;
		default: ucChar = 0xFE;
	}
	return ucChar;
}