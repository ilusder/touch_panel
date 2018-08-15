#include "Lcd_Graph.h"
#include "font.h"
//LCD graphic functions


void Lcd_Go_XY (int x, int y)
{
Lcd_Com_Data (0x0020,x);
Lcd_Com_Data (0x0021,y);
}

void Lcd_Fill_Screen (int color)
{
uint32_t j=76800;
Lcd_Go_XY (0, 0);
Lcd_Com (0x0022);
while(j)
	{
	Lcd_Data (color);
	j--;
	}
}

void Lcd_Put_Pix (int x, int y, int col)
{
Lcd_Go_XY (x,y);
Lcd_Com_Data (0x0022,col);
}
 //Алгоритм Брезентхема
void Lcd_Draw_Line (int x1, int y1, int x2, int y2, int color)
{
  int x, y, dx, dy;
  if(y1==y2)
  {
    if(x1<=x2) x=x1;
    else
    {
      x=x2;
      x2=x1;
    }
    
    while(x <= x2)
    {
      Lcd_Put_Pix (x,y1,color);
      x++;
    }
    return;
  }
  
  else if(y1>y2) dy=y1-y2;
  else dy=y2-y1;
  
  if(x1==x2)
  {
    if(y1<=y2) y=y1;
    else
    {
      y=y2;
      y2=y1;
    }
    
    while(y <= y2)
    {
      Lcd_Put_Pix (x1,y,color);
      y++;
    }
    return;
  }
  
  else if(x1 > x2)
  {
    dx=x1-x2;
    x = x2;
    x2 = x1;
    y = y2;
    y2 = y1;
  }
  else
  {
    dx=x2-x1;
    x = x1;
    y = y1;
  }
  if(dx == dy)
  {
    while(x <= x2)
    {
      x++;
      if(y>y2) y--;
      
      else y++;
      Lcd_Put_Pix (x,y,color);
    }
  } 
  else
  {
    Lcd_Put_Pix (x, y, color);
    if(y < y2)
    {
      if(dx > dy)
      {
        s16 p = dy * 2 - dx;
        s16 twoDy = 2 * dy;
        s16 twoDyMinusDx = 2 * (dy - dx);
        while(x < x2)
        {
          x++;
          if(p < 0) p += twoDy;
          else
          {
            y++;
            p += twoDyMinusDx;
          }
          Lcd_Put_Pix (x, y,color);
        }
      }
      else
      {
        s16 p = dx * 2 - dy;
        s16 twoDx = 2 * dx;
        s16 twoDxMinusDy = 2 * (dx - dy);
        while(y < y2)
        {
          y++;
          if(p < 0) p += twoDx;
          else
          {
            x++;
            p+= twoDxMinusDy;
          }
          Lcd_Put_Pix (x, y, color);
        }
      }
    }
    else
    {
      if(dx > dy)
      {
        s16 p = dy * 2 - dx;
        s16 twoDy = 2 * dy;
        s16 twoDyMinusDx = 2 * (dy - dx);
        while(x < x2)
        {
          x++;
          if(p < 0) p += twoDy;
          else
          {
            y--;
            p += twoDyMinusDx;
          }
          Lcd_Put_Pix (x, y,color);
        }
      }
      else
      {
        s16 p = dx * 2 - dy;
        s16 twoDx = 2 * dx;
        s16 twoDxMinusDy = 2 * (dx - dy);
        while(y2 < y)
        {
          y--;
          if(p < 0) p += twoDx;
          else
          {
            x++;
            p+= twoDxMinusDy;
          }
          Lcd_Put_Pix (x, y,color);
        }
      }
    }
  }
}

void LCD_Draw_Circle(int cx,int cy,int r,int color,u8 fill)
{
  int x=0,y=r;
  s16 delta=3-(r<<1),tmp;
  
  while(y>x)
  {
    if(fill)
    {
      Lcd_Draw_Line(cx+x,cy+y,cx-x,cy+y,color);
      Lcd_Draw_Line(cx+x,cy-y,cx-x,cy-y,color);
      Lcd_Draw_Line(cx+y,cy+x,cx-y,cy+x,color);
      Lcd_Draw_Line(cx+y,cy-x,cx-y,cy-x,color);
    }
    else
    {
      Lcd_Put_Pix (cx+x,cy+y,color);
      Lcd_Put_Pix (cx-x,cy+y,color);
      Lcd_Put_Pix (cx+x,cy-y,color);
      Lcd_Put_Pix (cx-x,cy-y,color);
      Lcd_Put_Pix (cx+y,cy+x,color);
      Lcd_Put_Pix (cx-y,cy+x,color);
      Lcd_Put_Pix (cx+y,cy-x,color);
      Lcd_Put_Pix (cx-y,cy-x,color);
    }
    x++;
    if(delta>=0)
    {
      y--;
      tmp=(x<<2);
      tmp-=(y<<2);
      delta+=(tmp+10);
    }
    else delta+=((x<<2)+6);             
  }
}
#include "Lcd_Graph.h"

void Lcd_Colors (void)
{
int i;
Lcd_Go_XY (0,0);
Lcd_Com (0x0022);
i=9600;
while(i--) Lcd_Data (RGB(255,255,255));		  //white
i=9600;
while(i--) Lcd_Data (RGB(255,255,0));		    //yellow
i=9600;
while(i--)
	{
	Lcd_Data (RGB(0,255,255));		    //goluboy
	}
i=9600;
while(i--)
	{
	Lcd_Data (RGB(0,255,0));		//green
	}
i=9600;
while(i--)
	{
	Lcd_Data (RGB(255,0,255));	   //siren
	}
i=9600;
while(i--)
	{
	Lcd_Data (RGB(255,0,0));	   //red
	}
i=9600;
while(i--)
	{
	Lcd_Data (RGB(128,128,128));	   //grey
	}
i=9840;
while(i--)
	{
	Lcd_Data (RGB(0,0,0));	        //siren
	}
}

//-----------------------------------------------------------------------------
//                       Fonts 8x16
// x,y - begin position
// c - char for out
// t_color - font color
// b_color - background color
//-----------------------------------------------------------------------------
void LCD_WriteChar_8x16(int x, int y, char c, int t_color, int b_color)
{
  unsigned char tmp_char=0;
  uint8_t i,j;
  for ( i=0;i<16;i++) 
  {
    tmp_char = Font_8x16[((c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01) Lcd_Put_Pix (x+j,y+i,t_color); // вставить color
      else Lcd_Put_Pix (x+j,y+i,b_color); // вставить background       
    } 
  }
}

void Lcd_String_8X16 (char *s, int x, int y, int t_color, int b_color)
	{
	while  (*s!='\0')
		{ 
		LCD_WriteChar_8x16 (x,y,*s++,t_color,b_color);
		x += 8; // next char
		}
	}

void Lcd_UInt32_Out (int32_t chisel, int  x, int y, int color, int b_color)
{
	unsigned char tmp[10];
	int i = 0, j = 0;
//	FONT_WITH_8_16 is font wihgt
//build array of chars (with digits)
	if (chisel < 0)	 
	{
		chisel = (~chisel)+1;
		LCD_WriteChar_8x16 (x+(j++)*FONT_WITH_8_16, y, '-', color, b_color);
	}
	while (chisel)
		{
		tmp[i] = (chisel % 10) + 0x30;  //Ascii
		chisel /= 10;
		i++;
		}
//print all chars
	while ((--i)+1)
	{
	LCD_WriteChar_8x16 (x+(j++)*FONT_WITH_8_16, y, tmp[i], color, b_color);
	} 
	if (j > 0) LCD_WriteChar_8x16(x+(j++)*FONT_WITH_8_16, y, ' ',  RGB (255,0,0), RGB (0,0,0));
}
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
