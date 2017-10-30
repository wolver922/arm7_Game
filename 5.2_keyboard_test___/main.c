/*********************************************************************************************
* Fileㄩ	main.c
* Author:	embest
* Descㄩ	c main entry
* History:	
*********************************************************************************************/
#include "44blib.h"
#include "44b.h"
#include "lcd.h"
#include "bmp.h"
/*------------------------------------------------------------------------------------------*/
/*	 								extern function						 				    */
/*------------------------------------------------------------------------------------------*/
extern UINT8T keyboard_test();

/*------------------------------------------------------------------------------------------*/
/*	 								function declare						 				    */
/*------------------------------------------------------------------------------------------*/
void Main(void);
void lcd_farm(void);
void adc_test_speed(void);
int set(UINT8T,int);
/*********************************************************************************************
* name:		main
* func:		c code entry
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
//void lcd(void);

void Main()
{
    sys_init();        /* Initial 44B0X's Interrupt,Port and UART */
	iic_init();//鍵盤初始化
	
	lcd_farm();//螢幕初始化
	adc_test_speed_init();//旋鈕初始化
	int count;
	count = 0;
	int key;
	key = 0;
	int speed;
	speed = 1;
	int count2;
	count2 = 0;
	int liftkey,rightkey;
	int Disc2;
	Disc2 = 0;
	/*while( uart_getch() != '\n' )
    {
	    rADCCON = 0x1 | (0x2<<2);	                            // AIN2
		while( !(rADCCON&0x40) );	                            // wait  conversion finish
		speed = adc_test_speed();//旋鈕
		uart_printf( "Ain2 = %d\n", speed );
	}*/
	int Disc = rand()%5;
	while(1){
	//Disc = rand()%5;
		for(count = 0;count<240;count++){
////////
			
			rADCCON = 0x1 | (0x2<<2);	                            // AIN2
			while( !(rADCCON&0x40) );	                            // wait  conversion finish
			
			speed = rADCDAT/75;
			uart_printf( "Ain2 = %d\n", speed );
			uart_printf( "Ain2 = 0x%03x\n", rADCDAT );
	count = count+speed;
		
			if(count == 0)
				Disc = rand()%5;
				//Disc = 0;
			key = set(keyboard_test(),key);
			switch(key){
				case 0:
					liftkey = 0;
					rightkey = 63;
					break;
				case 1:
					liftkey = 66;
					rightkey = 127;
					break;
				case 2:
					liftkey = 130;
					rightkey = 191;
					break;
				case 3:
					liftkey = 194;
					rightkey = 255;
					break;
				case 4:
					liftkey = 258;
					rightkey = 320;
					break;
			}//switch(key)
			////////////////////////////////////////////////////////////////
			switch(Disc){
			case 0:
			lcd_clr_rect(0,0+count,63,5+count,BLUE);
			if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,BLACK);
			delay(600);
			lcd_clr_rect(0,0+count,63,5+count,WHITE);
			if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,RED);
	lcd_clr_rect(0,200,320,215,0x80);
			break;
			case 1:
			lcd_clr_rect(66,0+count,127,5+count,GREEN);
			if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,BLACK);
			delay(600);
			lcd_clr_rect(66,0+count,127,5+count,WHITE);
			if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,RED);
	lcd_clr_rect(0,200,320,215,0x80);
			break;
			case 2:
			lcd_clr_rect(130,0+count,191,5+count,0x2b);
			if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,BLACK);
			delay(600);
			lcd_clr_rect(130,0+count,191,5+count,WHITE);
				if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,RED);
	lcd_clr_rect(0,200,320,215,0x80);
			break;
			case 3:
			lcd_clr_rect(194,0+count,255,5+count,0xa2);
		if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,BLACK);
			delay(600);
			lcd_clr_rect(194,0+count,255,5+count,WHITE);
			if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,RED);
	lcd_clr_rect(0,200,320,215,0x80);
			break;
			case 4:
			lcd_clr_rect(258,0+count,320,5+count,0x0c);
			if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,BLACK);
			delay(600);
			lcd_clr_rect(258,0+count,320,5+count,WHITE);
			if(key != 10)lcd_clr_rect(liftkey,200,rightkey,215,RED);
	lcd_clr_rect(0,200,320,215,0x80);
			break;
			}//switch(Disc)
			if(200 < count && count < 230 && Disc == key)//215 < count && count < 220 
			{
				lcd_clr_rect(liftkey,200,rightkey,215,BLUE);
				delay(600);
				count = -1-speed;
			}
			////////////////////////////////////////////////////////////////
			key = 10;//從設KEY
			
			////
			count2++;
			if(count2 = 240)
				count2 = 0;
			}//for(count = 0;count<240;count++){

		}//while(1){
	uart_printf("\n\rKeyboard Test Example\n");
	while(1);
}

void lcd_farm()
{
	lcd_init();									// initial LCD controller
	lcd_clr();									// clear screen
	LCD_D_ON;
	lcd_clr_rect(0,200,320,215,0x80);
	int i;
	for(i = 1 ; i < 5 ; i++){
	lcd_clr_rect(64*i,0,64*i+1,240,BLACK);
	}
}
void adc_test_speed_init(void)
{
	int nAdcpsr;
	rCLKCON = 0x7ff8;
	rADCCON = 0x1 | (0<<2);                                     // Enable ADC
	delay(100);                                                 // delay for 10ms for ADC reference voltage stabilization.
}

int set(UINT8T ucChar,int key)//傳出一個數值再跌牌DELAY那邊一起DELAY
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
			key = 0;
			break;
		case 26: 
			key = 1;
			 break;
		case 27: 
			key = 2;
			 break;
		case 28: 
			key = 3;
			 break;
		case 29: 
			key = 4;
			break;
		default: ucChar = 0xFE;
	}
	return key;
}