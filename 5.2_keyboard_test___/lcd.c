/*********************************************************************************************
* File：	lcd.c
* Author:	embest	
* Desc：	LCD control and display functions
* History:	
*			H.T.Zhang, Programming modify, September 12, 2005
*********************************************************************************************/
#include "lcd.h"
#include "bmp.h"
#include "44b.h"

/*------------------------------------------------------------------------------------------*/
/*	 								constant define							 			    */
/*------------------------------------------------------------------------------------------*/
#define XWIDTH 				6
#define LCD_STN256_COLOR
#define ASCII6x8

/*------------------------------------------------------------------------------------------*/
/*	 								global variables						 			    */
/*------------------------------------------------------------------------------------------*/
UINT32T g_unLcdActiveBuffer[LCD_YSIZE][LCD_XSIZE/4];

/*------------------------------------------------------------------------------------------*/
/*	 								extern variables						 			    */
/*------------------------------------------------------------------------------------------*/
extern const UINT8T g_ucBitmap[][76800];
extern UINT8T g_ucAscii8x16[];
#ifdef ASCII6x8
extern UINT8T g_ucAscii6x8[];
#endif
//#ifdef CHINESE_VERSION
#ifdef HZK12
extern const UINT8T g_auc_hzk12[];
#endif
#ifdef HZK16
extern const UINT8T g_ucHZK16[];
#endif
#ifdef HZK24
extern const UINT8T g_auc_hzk24[];
#endif
//#endif

/*********************************************************************************************
* name:		lcd_init()
* func:		Initialize LCD Controller
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_init(void)
{     
	rDITHMODE = 0x12210;
	rDP1_2  = 0xa5a5;      
	rDP4_7  = 0xba5da65;
	rDP3_5  = 0xa5a5f;
	rDP2_3  = 0xd6b;
	rDP5_7  = 0xeb7b5ed;
	rDP3_4  = 0x7dbe;
	rDP4_5  = 0x7ebdf;
	rDP6_7  = 0x7fdfbfe;

	// disable,8B_SNGL_SCAN,WDLY=16clk,WLH=16clk,
	rLCDCON1 = (0x0)|(2<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_COLOR<<12);
	// LINEBLANK=10 (without any calculation) 
	rLCDCON2 = (LINEVAL)|(HOZVAL_COLOR<<10)|(10<<21);  
	rLCDCON3 = 0;

	// 256-color, LCDBANK, LCDBASEU
	rLCDSADDR1 = (0x3<<27) | ( ((unsigned int)g_unLcdActiveBuffer>>22)<<21 ) | M5D((unsigned int)g_unLcdActiveBuffer>>1);
	rLCDSADDR2 = M5D((((unsigned int)g_unLcdActiveBuffer+(SCR_XSIZE*LCD_YSIZE))>>1)) | (MVAL<<21);
	rLCDSADDR3 = (LCD_XSIZE/2) | ( ((SCR_XSIZE-LCD_XSIZE)/2)<<9 );

	//The following value has to be changed for better display.
	rREDLUT  =0xfdb96420; // 1111 1101 1011 1001 0110 0100 0010 0000
	rGREENLUT=0xfdb96420; // 1111 1101 1011 1001 0110 0100 0010 0000
	rBLUELUT =0xfb40;     // 1111 1011 0100 0000

	rLCDCON1=(0x1)|(2<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_COLOR<<12);
	rPDATE=rPDATE&0x0e;
	lcd_clr();
}

/*********************************************************************************************
* name:		lcd_clr()
* func:		clear LCD screen
* para:		none 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_clr(void)
{
	UINT32T i;
	UINT32T *pDisp = (UINT32T*)g_unLcdActiveBuffer;
	
	for (i = 0; i < (SCR_XSIZE * SCR_YSIZE  / 4); i++)
	{
		*pDisp++ = ALLWHITE;
	}
}
#ifdef LCD_STN256_COLOR
/*********************************************************************************************
* name:		lcd_clr_rect()
* func:		fill appointed area with appointed color
* para:		usLeft,usTop,usRight,usBottom -- area's rectangle acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:	also as clear screen function 
*********************************************************************************************/
void lcd_clr_rect(UINT16T usLeft, UINT16T usTop, UINT16T usRight, UINT16T usBottom, UINT8T ucColor)
{
	UINT32T i, j;
	UINT8T  *pDisp = (UINT8T*)g_unLcdActiveBuffer;
	
	for(i=usTop;i<=usBottom;i++)
	{
		for(j=usLeft;j<=usRight;j++)
		{
			LCD_PutPixel(j, i, ucColor);
		}
	}	

}
#endif

#ifdef LCD_STN256_GRAY
/*********************************************************************************************
* name:		lcd_clr_rect()
* func:		fill appointed area with appointed color
* para:		usLeft,usTop,usRight,usBottom -- area's rectangle acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:	also as clear screen function 
*********************************************************************************************/
void lcd_clr_rect(UINT16T usLeft, UINT16T usTop, UINT16T usRight, UINT16T usBottom, UINT8T ucColor)
{
	UINT16T i=0;
	UINT16T k,l;
	UINT16T m;
	
	UINT32T ulColor = (ucColor << 28) | (ucColor << 24) | (ucColor << 20) | (ucColor << 16) | 
					  (ucColor << 12) | (ucColor << 8) | (ucColor << 4) | ucColor;
	
	if((usRight-usLeft)<=8)
	{
		for(i=usTop;i<=usBottom;i++)
		{
			for(m=usLeft;m<=usRight;m++)
			{
				LCD_PutPixel(m, i, ucColor);
			}
		}	
		return;
	}

	if((usLeft%8)==0)
		k=usLeft;
	else
		k=(usLeft/8)*8+8;

	if(( usRight%8)==0)
		l= usRight;
	else
		l=( usRight/8)*8;

	for(i=usTop;i<=usBottom;i++)
	{
		for(m=usLeft;m<=(k-1);m++)
			LCD_PutPixel(m, i, ucColor);
		for(m=k;m<l;m+=8)
			(*(UINT32T*)(g_unLcdActiveBuffer + i * SCR_XSIZE / 2 + m / 2)) = ulColor;
		for(m=l;m<=usRight;m++)
			LCD_PutPixel(m, i, ucColor);
	}
}
#endif

/*********************************************************************************************
* name:		lcd_draw_box()
* func:		Draw rectangle with appointed color
* para:		usLeft,usTop,usRight,usBottom -- rectangle's acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_draw_box(UINT16T usLeft, UINT16T usTop, UINT16T usRight, UINT16T usBottom, UINT8T ucColor)
{
	lcd_draw_hline(usLeft, usRight,  usTop,    ucColor, 1);
	lcd_draw_hline(usLeft, usRight,  usBottom, ucColor, 1);
	lcd_draw_vline(usTop,  usBottom, usLeft,   ucColor, 1);
	lcd_draw_vline(usTop,  usBottom, usRight,  ucColor, 1);
}

/*********************************************************************************************
* name:		lcd_draw_line()
* func:		Draw line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1,usY1 -- line's end point coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_draw_line(UINT16T usX0, UINT16T usY0, UINT16T usX1, UINT16T usY1, UINT8T ucColor, UINT16T usWidth)
{
	UINT16T usDx;
	UINT16T usDy;
	UINT16T y_sign;
	UINT16T x_sign;
	UINT16T decision;
	UINT16T wCurx, wCury, wNextx, wNexty, wpy, wpx;

	if( usY0 == usY1 )
	{
		lcd_draw_hline (usX0, usX1, usY0, ucColor, usWidth);
		return;
	}
	if( usX0 == usX1 )
	{
		lcd_draw_vline (usY0, usY1, usX0, ucColor, usWidth);
		return;
	}
	usDx = abs(usX0 - usX1);
	usDy = abs(usY0 - usY1);
	if( ((usDx >= usDy && (usX0 > usX1)) ||
        ((usDy > usDx) && (usY0 > usY1))) )
    {
        GUISWAP(usX1, usX0);
        GUISWAP(usY1, usY0);
    }
    y_sign = (usY1 - usY0) / usDy;
    x_sign = (usX1 - usX0) / usDx;

    if( usDx >= usDy )
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDx >> 1);
             wCurx <= wNextx; wCurx++, wNextx--, decision += usDy )
        {
            if( decision >= usDx )
            {
                decision -= usDx;
                wCury += y_sign;
                wNexty -= y_sign;
            }
            for( wpy = wCury - usWidth / 2;
                 wpy <= wCury + usWidth / 2; wpy++ )
            {
                LCD_PutPixel(wCurx, wpy, ucColor);
            }

            for( wpy = wNexty - usWidth / 2;
                 wpy <= wNexty + usWidth / 2; wpy++ )
            {
                LCD_PutPixel(wNextx, wpy, ucColor);
            }
        }
    }
    else
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDy >> 1);
             wCury <= wNexty; wCury++, wNexty--, decision += usDx )
        {
            if( decision >= usDy )
            {
                decision -= usDy;
                wCurx += x_sign;
                wNextx -= x_sign;
            }
            for( wpx = wCurx - usWidth / 2;
                 wpx <= wCurx + usWidth / 2; wpx++ )
            {
                LCD_PutPixel(wpx, wCury, ucColor);
            }

            for( wpx = wNextx - usWidth / 2;
                 wpx <= wNextx + usWidth / 2; wpx++ )
            {
                LCD_PutPixel(wpx, wNexty, ucColor);
            }
        }
    }
}

/*********************************************************************************************
* name:		lcd_draw_hline()
* func:		Draw horizontal line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1 -- line's end point X-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_draw_hline(UINT16T usX0, UINT16T usX1, UINT16T usY0, UINT8T ucColor, UINT16T usWidth)
{
	UINT16T usLen;

    if( usX1 < usX0 )
    {
        GUISWAP (usX1, usX0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usX1 - usX0 + 1;
        while( (usLen--) > 0 )
        {
        	LCD_PutPixel(usX0 + usLen, usY0, ucColor);
        }
        usY0++;
    }
}

/*********************************************************************************************
* name:		lcd_draw_vline()
* func:		Draw vertical line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usY1 -- line's end point Y-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_draw_vline (UINT16T usY0, UINT16T usY1, UINT16T usX0, UINT8T ucColor, UINT16T usWidth)
{
	UINT16T usLen;

    if( usY1 < usY0 )
    {
        GUISWAP (usY1, usY0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usY1 - usY0 + 1;
        while( (usLen--) > 0 )
        {
        	LCD_PutPixel(usX0, usY0 + usLen, ucColor);
        }
        usX0++;
    }
}

/*********************************************************************************************
* name:		lcd_disp_ascii8x16()
* func:		display 8x16 ASCII character string 
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_disp_ascii8x16(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T * s)
{
	/*UINT16T i,j,k,x,y,xx;
	UINT8T qm;
	UINT32T ulOffset;
	INT8T ywbuf[16],temp[2];
    
	for( i = 0; i < strlen((const char *)s); i++ )
	{
		if( (UINT8T)*(s+i) >= 161 )
		{
			temp[0] = *(s + i);
			temp[1] = '\0';
			return;
		}
		else
		{
			qm = *(s+i);
			ulOffset = (UINT32T)(qm) * 16;		//Here to be changed tomorrow
			for( j = 0; j < 16; j ++ )
			{
				ywbuf[j] = g_ucAscii8x16[ulOffset + j];
            }

            for( y = 0; y < 16; y++ )
            {
            	for( x = 0; x < 8; x++ ) 
               	{
                	k = x % 8;
			    	if( ywbuf[y]  & (0x80 >> k) )
			       	{
			       		xx = x0 + x + i*8;
			       		LCD_PutPixel(xx, y + y0, (UINT8T)ForeColor);
			       	}
			   	}
            }
		}
	}*/
}

#ifdef ASCII6x8
/*********************************************************************************************
* name:		lcd_disp_ascii6x8()
* func:		display 6x8 ASCII character string 
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_disp_ascii6x8(UINT16T usX0, UINT16T usY0,UINT8T ForeColor, UINT8T* pucChar)
{
	/*UINT32T i,j;
	UINT8T  ucTemp;

	while( *pucChar != 0 )
	{
		for( i=0; i < 8; i++ )
		{
  			ucTemp = g_ucAscii6x8[(*pucChar) * 8 + i];
  			for( j = 0; j < 8; j++ )
  			{
  				if( (ucTemp & (0x80 >> j)) != 0 )
  				{
  					LCD_PutPixel(usX0 + i, usY0 + 8 - j, (UINT8T)ForeColor);
  				}  				
  			}
		}
		usX0 += XWIDTH;
		pucChar++;
	}*/
}
#endif

#ifdef HZK12
//*****************************************************************
//** 函数名:void lcd_disp_hz12(U16 x0, U16 y0, INT8U ForeColor, INT8U *s)
//** 输 入: x0--字符串的左上角x坐标
//		   y0--字符串的坐上角y坐标
//**          ForeColor--填充的颜色
//		   s--字符串数组指针
//** 输 出: Null
//** 功能描述:显示12x12汉字字符串
//** 全局变量:Null
//*****************************************************************
void lcd_disp_hz12(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T *s)
{
	INT16 i,j,k,x,y,xx;
	INT8U qm,wm;
	S32 ulOffset;
	INT8 hzbuf[24],temp[2];

    	for(i = 0; i < strlen((const char*)s); i++)
    	{
    		if( (INT8U)*(s + i) < 161 )
    		{
    	    		/* AscII char */
    			temp[0] = *(s + i); 
    			temp[1] = '\0';    		
    			break;
    		}
    		else
    		{
    	    		/* HanZi */
    			qm = *(s + i) - 161;
    			wm = *(s + i + 1) - 161;
    			ulOffset = (S32)(qm * 94 + wm) * 24;

            		for (j = 0; j < 24; j ++)
            		{
            			hzbuf[j] = g_auc_hzk12[ulOffset + j];
             		}
             		
    			/* Get the HZ ZiMo By X */
    			for(y = 0; y < 12; y++)
    			{
    				for(x = 0; x < 16; x++)
    				{
    			   		if(x < 12)
    			   		{
    			   	 		k = x % 8;    			   	 		
						if (hzbuf[y * 2 + x / 8]  & (0x80 >> k))
						{
	    			   	 		xx = x0 + x + i * 6;
					     		LCD_PutPixel( xx, y + y0, ForeColor);			       	
						}
    			   		}
    				}
    			} 
    			/* One HZ Ocupy 2 bytes, So i++ */
    			i++;    		
    		}
    	}
}
#endif

#ifdef HZK16
/*********************************************************************************************
* name:		lcd_disp_hz16()
* func:		display chinese character string in 16x16 dot array
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_disp_hz16(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T *s)
{
	UINT16T i,j,k,x,y,xx;
	UINT8T qm,wm;
	UINT32T ulOffset;
	INT8T hzbuf[32],temp[2];

	for( i = 0; i < strlen((const char*)s); i++ )
	{
		if( ((UINT8T)(*(s+i))) < 161 )
		{
			temp[0] = *(s+i);
			temp[1] = '\0';
			break;
		}
		else
		{
			qm = *(s+i) - 161;
    		wm = *(s + i + 1) - 161;
       		ulOffset = (UINT32T)(qm * 94 + wm) * 32;
			for( j = 0; j < 32; j ++ )
            {
            	hzbuf[j] = g_ucHZK16[ulOffset + j];
            }
            for( y = 0; y < 16; y++ )
            {
	        	for( x = 0; x < 16; x++ ) 
	            {
                	k = x % 8;
				   	if( hzbuf[y * 2 + x / 8]  & (0x80 >> k) )
				    {
				    	xx = x0 + x + i * 8;
				    	LCD_PutPixel( xx, y + y0, (UINT8T)ForeColor);
				    }
			   	}
           	}
		    i++;
		}
	}
}
/*********************************************************************************************
* name:		lcd_disp_str16()
* func:		display 8x16 ASCII character string and 16x16 chinese character string 
* para:		usX0,usY0 -- character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_disp_str16(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T *s)
{
	UINT16T i,j,k,x,y,xx=0,yy,ch=0;
	UINT8T qm,wm;
	UINT32T ulOffset,ulPadRow;
	INT8T ywbuf[16],hzbuf[32];//,temp[2];

	for( i = 0; i < strlen((const char*)s); i++ )
	{
		if( ((UINT8T)(*(s+i))) < 161 )
		{
			ch++;
			if(xx > 300)
			{
				ch = 0;
				x0 = 15;
				y0 += ulPadRow;
			}
			qm = *(s+i);
			ulOffset = (UINT32T)(qm) * 16;		//Here to be changed tomorrow
			for( j = 0; j < 16; j ++ )
			{
				ywbuf[j] = g_ucAscii8x16[ulOffset + j];
            		}

            		for( y = 0; y < 16; y++ )
            		{
            			for( x = 0; x < 8; x++ ) 
               			{
                			k = x % 8;
			    		if( ywbuf[y]  & (0x80 >> k) )
			       		{
			       			xx = x0 + x + ch*8;
			       			yy = y + y0;
			       			LCD_PutPixel(xx, yy, (UINT8T)ForeColor);
			       		}
			   	}
            		}
		}
		else
		{
			ch++;
			if(xx > 288) 
			{
				ch = 0;
				x0 = 15;
				y0 += ulPadRow;
			}
			qm = *(s+i) - 161;
    		wm = *(s + i + 1) - 161;
       		ulOffset = (UINT32T)(qm * 94 + wm) * 32;
			for( j = 0; j < 32; j ++ )
            		{
            			hzbuf[j] = g_ucHZK16[ulOffset + j];
            		}
            		for( y = 0; y < 16; y++ )
            		{
	        		for( x = 0; x < 16; x++ ) 
	            		{
                			k = x % 8;
				   	if( hzbuf[y * 2 + x / 8]  & (0x80 >> k) )
				    	{
				    		xx = x0 + x + ch * 8;
				    		yy = y + y0;
				    		LCD_PutPixel( xx, yy, (UINT8T)ForeColor);
				    	}
			   	}
           		}
	    	i++;
	    	ch++;
		}
	}
}
#endif

#ifdef HZK24
//*****************************************************************
//** 函数名:void lcd_disp_hz24(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T *s)
//** 输 入: x0--字符串的左上角x坐标
//		   y0--字符串的左上角y坐标
//**          ForeColor--填充的颜色
//		   s--字符串数组指针
//** 输 出: Null
//** 功能描述:显示24x24汉字字符串
//** 全局变量:Null
//*****************************************************************
void lcd_disp_hz24(UINT16T x0, UINT16T y0, UINT8T ForeColor, UINT8T *s)
{
	INT16T i,j,k,x,y,xx;
	UINT8T qm,wm;
	INT32T ulOffset;
	UINT8T hzbuf[72],temp[2];

	for(i = 0; i < strlen((const char*)s); i++)
	{
		if(((UINT8T)(*(s+i))) < 161)
		{
			temp[0] = *(s+i);
			temp[1] = '\0';
			break;
		}
		else
		{
			qm = *(s+i) - 176;//161;
	    	wm = *(s+i + 1) - 161;
            ulOffset = (INT32T)(qm * 94 + wm) * 72;
            for (j = 0; j < 72; j ++)
            {
            	hzbuf[j] = g_auc_hzk24[ulOffset + j];
            }
            for(y = 0; y < 24; y++)
            {
				for(x = 0; x < 24; x++) 
				{
					k = x % 8;
					if (hzbuf[y * 3 + x / 8]  & (0x80 >> k))
			       	{
			       		xx = x0 + x + i*12;
			       		LCD_PutPixel( xx, y + y0, (UINT8T)ForeColor);
			       	}
				}
			}
            	/*	for(x=0;x<24;x++)
            		{
            			for(y=0;y<24;y++)
            			{
   							k = y%8;
    						if( hzbuf[x*3+y/8] & (0x80 >> k) )
    					{
    						xx = x0+x+i*8;
    						LCD_PutPixel(xx,y+y0,(UINT8T)ForeColor);
    					}
            			}
            		}*/
			i++;
		}
	}     	
}
#endif
/*********************************************************************************************
* name:		color_lcd_test()
* func:		LCD test function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void color_lcd_test(void)
{
//	f_nInterface = 15;
	
/*	int i,j;
	
	lcd_init();									// initial LCD controller
	lcd_clr();									// clear screen

#ifdef CHINESE_VERSION
	lcd_disp_hz16(10,10,BLUE,"英蓓特三星实验平台");
#else
	lcd_disp_ascii8x16(10,10,BLUE,"Embest S3CEV40 ");
#endif
	lcd_disp_ascii8x16(10,24,GREEN-55,"ShenZhen Embest Info&Tech Co.,LTD");
	
	
	lcd_draw_box(10,40,310,230,RED);
	lcd_draw_box(20,45,300,225,GREEN);
	lcd_draw_box(30,50,290,220,BLUE);
	lcd_draw_box(40,55,280,215,GREEN-55);

	LCD_D_ON;
	delay(10000);

	for(j=0;j<2;j++)
	{
		for (i = 0; i < 3; i++)
		{
			bitmap_view320x240x256((UINT8T*)(g_ucBitmap[i]));
			delay(5000);
		}
	}*/
}
