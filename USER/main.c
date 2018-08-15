/*******************************************************************/

#include "stm32f10x.h"
#define F_CPU 72000000UL
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "rs232.h"
#include "Lcd_Graph.h"
#include "tsc2046_touch.h"


void InitAll (void)
{
// Выставляем тактирование в APB2
RCC->APB2ENR 	|= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;	 //LED Port B + Port A + Alternative func Port A

// Конфигурируем CRL регистры LED. 
GPIOB->CRL	&= ~GPIO_CRL_CNF5;		// Сбрасываем биты CNF для бита 5. Режим 00 - Push-Pull 
GPIOB->CRL 	|= GPIO_CRL_MODE5_0;	// Выставляем бит MODE0 для пятого пина. Режим MODE01 = Max Speed 10MHz
}

//Interupts func
// Int Vectors
void EXTI1_IRQHandler(void)
{
 
}

int main(void)
{
int j;
struct point xypoint;
struct calibration calib;
calib.cali_A = 13136;
calib.cali_B = 1600;
calib.cali_C = -78830578;
calib.cali_D = 437;
calib.cali_E = 11652;
calib.cali_F = -13663692;
InitAll ();
serial2_setup();
InitLcd ();
spi_touch_conf ();
Lcd_Fill_Screen ( RGB(0,255,0) );
Lcd_String_8X16 ("STMF103VET6", 20, 2, BLUE, GREEN);
USART2_Send_Str ("STMF103VET6");
//Touch_Calib (&calib);
//i=20;
//k=100;
//j=150;

Delay (30000000);
j=60;
while (j--) LCD_Draw_Circle (80+j,200-j,30+j,BLUE-j,0);
	Lcd_UInt32_Out (calib.cali_A, 0, 20, RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (calib.cali_B, 0, 35, RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (calib.cali_C, 0, 50, RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (calib.cali_D, 0, 65, RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (calib.cali_E, 0, 80, RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (calib.cali_F, 0, 95, RGB (255,0,0), RGB (0,0,0));
Delay (30000000);
Lcd_Fill_Screen ( RGB(255,255,255) );
//Lcd_Draw_Line (10,1,20,250,RED);
//LCD_Draw_Circle (100, 10, 5, RED, 1);
while (1) 
	{
	spi_touch_chat (15,&xypoint);		//10011111
	LCD_WriteChar_8x16(0, 0, 'X',  RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (xypoint.d_in_x, 8, 0, RGB (255,0,0), RGB (0,0,0));
	LCD_WriteChar_8x16(100, 0, 'Y',  RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (xypoint.d_in_y, 108, 0, RGB (255,0,0), RGB (0,0,0));
	if (xypoint.d_in_x) 
		{
		touch_correct (&xypoint,&calib);
			LCD_WriteChar_8x16(0, 15, 'x',  RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (xypoint.d_in_x, 8, 15, RGB (255,0,0), RGB (0,0,0));
	LCD_WriteChar_8x16(100, 15, 'y',  RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (xypoint.d_in_y, 108, 15, RGB (255,0,0), RGB (0,0,0));
		
		if (xypoint.d_in_x >= 230 && xypoint.d_in_y >= 290) Lcd_Fill_Screen ( RGB(255,255,255) );
		else 
			{
			LCD_Draw_Circle (xypoint.d_in_x, xypoint.d_in_y, 3, RED, 1);
//			Lcd_Put_Pix (xypoint.d_in_x, xypoint.d_in_y, BLUE);
//			Lcd_Put_Pix (xypoint.d_in_x+1, xypoint.d_in_y, BLUE);
//			Lcd_Put_Pix (xypoint.d_in_x, xypoint.d_in_y+1, BLUE);
//			Lcd_Put_Pix (xypoint.d_in_x+1, xypoint.d_in_y+1, BLUE);
			}
		}
	}
}

