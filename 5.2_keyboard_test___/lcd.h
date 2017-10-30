/*********************************************************************************************
* File£º	LCD.H
* Author:	embest	
* Desc£º	LCD common macro define and function declare
* History:	
*********************************************************************************************/
#ifndef __LCD_H__
#define __LCD_H__

#include	"def.h"     

/*------------------------------------------------------------------------------------------*/
/*	 								macro define						 				    */
/*------------------------------------------------------------------------------------------*/
#define SCR_XSIZE 			(320)  
#define SCR_YSIZE 			(240)
#define LCD_XSIZE 			(320)
#define LCD_YSIZE 			(240)
#define COLOR_NUMBER 		256

#define HOZVAL				(LCD_XSIZE/4-1)
#define HOZVAL_COLOR		(LCD_XSIZE*3/8-1)
#define LINEVAL				(LCD_YSIZE-1)
#define MVAL				(13)
#define M5D(n) 				((n) & 0x1fffff)
#define MVAL_USED 			0
#define CLKVAL_COLOR 		(4) 

#define BLACK 				0x0
#define ALLBLACK 			((BLACK << 24) | (BLACK << 16) | (BLACK << 8) | BLACK)
#define WHITE 				0xff
#define ALLWHITE 			((WHITE << 24)  | (WHITE << 16)  | (WHITE << 8)  | WHITE)
#define RED 				0xe0
#define GREEN 				0x1c
#define BLUE 				0x03
#define LCD_D_OFF 			(*(int *)0x1d20020) &= ~(1 << 6); // rPDATD
#define LCD_D_ON 			(*(int *)0x1d20020) |= (1 << 6); 

#define LCD_PutPixel(x, y, c)\
	g_unLcdActiveBuffer[(y)][(x)/4]=(( g_unLcdActiveBuffer[(y)][(x)/4] & (~(0xff000000>>((x)%4)*8)) ) | ( (c)<<((4-1-((x)%4))*8) ));
#define LCD_ActivePutPixel(x, y, c)\
	g_unLcdActiveBuffer[(y)][(x)/4]=(( g_unLcdActiveBuffer[(y)][(x)/4] & (~(0xff000000>>((x)%4)*8)) )\
	| ( (c)<<((4-1-((x)%4))*8) ));

#define GUISWAP(a, b) {a^=b; b^=a; a^=b;}

/*------------------------------------------------------------------------------------------*/
/*	 								external variables					 				    */
/*------------------------------------------------------------------------------------------*/
extern UINT32T g_unLcdActiveBuffer[LCD_YSIZE][LCD_XSIZE/4];

/*------------------------------------------------------------------------------------------*/
/*	 								function declare						 				    */
/*------------------------------------------------------------------------------------------*/
void  lcd_init(void);
void  lcd_clr(void);
void  lcd_test(void);
void  lcd_clr_rect(UINT16T usLeft, UINT16T usTop, UINT16T usRight, UINT16T usBottom, UINT8T ucColor);
void  lcd_draw_box(UINT16T usLeft, UINT16T usTop, UINT16T usRight, UINT16T usBottom, UINT8T ucColor);
void  lcd_draw_line(UINT16T usX0, UINT16T usY0, UINT16T usX1, UINT16T usY1, UINT8T ucColor, UINT16T usWidth);
void  lcd_draw_hline(UINT16T usX0, UINT16T usX1, UINT16T usY0, UINT8T ucColor, UINT16T usWidth);
void  lcd_draw_vline(UINT16T usY0, UINT16T usY1, UINT16T usX0, UINT8T ucColor, UINT16T usWidth);
void  lcd_disp_ascii6x8(UINT16T usX0, UINT16T usY0,UINT8T ForeColor, UINT8T* pucChar);
void  lcd_disp_ascii8x16(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T* s);
void  lcd_dsp_hz16(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T *s);
#endif
