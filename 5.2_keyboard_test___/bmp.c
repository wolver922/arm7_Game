/*********************************************************************************************
* File£º	lcd.c
* Author:	embest	
* Desc£º	LCD control and display functions
* History:	
*********************************************************************************************/
#include "bmp.h"
#include "def.h"
#include "lcd.h"

/*********************************************************************************************
* name:		bitmap_view320x240x256
* func:		display bitmap
* para:		pBuffer -- input, bitmap data
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void bitmap_view320x240x256(UINT8T *pBuffer)
{
	UINT32T i, j;
	UINT32T *pView = (UINT32T*)g_unLcdActiveBuffer;

	for (i = 0; i < SCR_XSIZE * SCR_YSIZE / 4; i++)
	{
		*pView = ((*pBuffer) << 24) + ((*(pBuffer+1)) << 16) + ((*(pBuffer+2)) << 8) + (*(pBuffer+3));
		pView++;
		pBuffer += 4;
	}
}

