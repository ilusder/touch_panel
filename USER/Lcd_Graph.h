#include "lcd_fsmc.h"
#include "colors.h"


#define FONT_HIGH_8_16 16
#define FONT_WITH_8_16 8
//LCD graphic functions


void Lcd_Go_XY (int x, int y);
void Lcd_Fill_Screen (int color);
void Lcd_Put_Pix (int x, int y, int col);
 //Алгоритм Брезентхема
void Lcd_Draw_Line (int x1, int y1, int x2, int y2, int color);
void LCD_Draw_Circle(int cx,int cy,int r,int color,u8 fill);
void Lcd_Colors (void);
//-----------------------------------------------------------------------------
//                       Fonts 8x16
// x,y - begin position
// c - char for out
// t_color - font color
// b_color - background color
//-----------------------------------------------------------------------------
void LCD_WriteChar_8x16(int x, int y, char c, int t_color, int b_color);
void LCD_WriteChar_14x12(int x, int y, char c, int t_color, int b_color);
void Lcd_String_8X16 (char *s, int x, int y, int t_color, int b_color);
void Lcd_String_14X12 (char *s, int x, int y, int t_color, int b_color);
void Lcd_UInt32_Out (int chisel, int  x, int y, int color, int b_color);

   /*
void Lcd_Bitmap (int *pic, int with)
{
int str=70, col;
Lcd_Go_XY (0,0);
while (str--)		   //строки - циклы
	{
	col = 320; // количество пикселей
	while (col--) 
		{
		if (col <= with) Lcd_Com_Data (0x0022,*pic++);
		else  Lcd_Com_Data (0x0022,BLACK);
		}
	}
}	 */
