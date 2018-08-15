#include "tsc2046_touch.h"
#include "colors.h"

void spi_touch_chat (uint8_t i,struct point *xypoint)	//i - how many scans to do (resolution)
{
int tmpx=0, tmpy=0;
(*xypoint).d_in_x = get_touch (GET_X) / DIV_TOUCH;
(*xypoint).d_in_y = get_touch (GET_Y) / DIV_TOUCH;
if ((*xypoint).d_in_y == 0) (*xypoint).d_in_x = 0;
if ((*xypoint).d_in_x == 0) (*xypoint).d_in_y = 0;
while (i)
	{
	i--;
	tmpx = get_touch (GET_X) / DIV_TOUCH;
	tmpy = get_touch (GET_Y) / DIV_TOUCH;
	if (tmpx == 0) tmpy = 0;
	if (tmpy == 0) tmpx = 0;
	if (!tmpx) 
		{
		(*xypoint).d_in_x = 0;
		(*xypoint).d_in_y = 0;
		break;
		}
	else 
		{
		(*xypoint).d_in_x = ((*xypoint).d_in_x + tmpx) / 2;
		(*xypoint).d_in_y = ((*xypoint).d_in_y + tmpy) / 2;
		}
	}
	if ((*xypoint).d_in_x)			//normalize direction of ACD outs
		{
		(*xypoint).d_in_x = ADC_X_APP_MAX - (*xypoint).d_in_x;
		(*xypoint).d_in_y = ADC_Y_APP_MAX - (*xypoint).d_in_y;
		//calculate real coordinates
		}
	Delay (50);		//delay
}

void touch_correct (struct point *xypoint, struct calibration *calibr)
{
	int32_t in_x = (*xypoint).d_in_x;
	int32_t in_y = (*xypoint).d_in_y;
	(*xypoint).d_in_x = ((*calibr).cali_A * in_x + (*calibr).cali_B * in_y + (*calibr).cali_C) / RESCALE_FACTOR;
	(*xypoint).d_in_y = ((*calibr).cali_D * in_x + (*calibr).cali_E * in_y + (*calibr).cali_F) / RESCALE_FACTOR;
	if ((*xypoint).d_in_x >= TOUCH_X_MAX || (*xypoint).d_in_x < 0 || (*xypoint).d_in_y >= TOUCH_Y_MAX || (*xypoint).d_in_y < 0)
		{
		(*xypoint).d_in_x = 0;
		(*xypoint).d_in_y = 0;
		}
}

void Touch_Calib (struct calibration *calibr)
{
int i =0;
volatile double xd[3] = {(50 * TOUCH_X_MAX) / 100,  (90 * TOUCH_X_MAX) / 100, (10 * TOUCH_X_MAX) / 100};
volatile double yd[3] = {(10 * TOUCH_Y_MAX) / 100, (50 * TOUCH_Y_MAX) / 100, (90 * TOUCH_Y_MAX) / 100};
uint32_t xt[3],yt[3];
volatile double temp1, temp2;
struct point xypoint;
volatile double cal_A = 0.0, cal_B = 0.0, cal_C = 0.0, cal_D = 0.0, cal_E = 0.0, cal_F = 0.0;

//get all coordinates to d_cal_X[] and d_cal_Y[]
Lcd_String_8X16 ("Calibrate Touch Screen", 20, 2, RED, GREEN);
USART2_Send_Str ("Calibrate Touch Screen");
while (i<3)
{
LCD_Draw_Circle (xd[i],yd[i],5,BLACK,1); //point i
while (1)
	{
	spi_touch_chat (250,&xypoint);
	if (xypoint.d_in_x)	   //if we have point coordinates 
		{
		xt[i] = xypoint.d_in_x;				//save point here
		yt[i] = xypoint.d_in_y;
		break;
		}
	}
	LCD_WriteChar_8x16(0, 0, 'X',  RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (xt[i], 8, 0, RGB (255,0,0), RGB (0,0,0));
	LCD_WriteChar_8x16(100, 0, 'Y',  RGB (255,0,0), RGB (0,0,0));
	Lcd_UInt32_Out (yt[i], 108, 0, RGB (255,0,0), RGB (0,0,0));
i++;
}
	//finish get coordinates
Lcd_Fill_Screen ( RGB(0,255,0) );
Lcd_String_8X16 ("Thanks. Wait...", 20, 20, RED, GREEN);

//A
temp1 = ((double) xd[0] * ((double) yt[1] - (double) yt[2])) + ((double) xd[1] * ((double) yt[2] - (double) yt[0])) + ((double) xd[2] * ((double) yt[0] - (double) yt[1]));
temp2 = ((double) xt[0] * ((double) yt[1] - (double) yt[2])) + ((double) xt[1] * ((double) yt[2] - (double) yt[0])) + ((double) xt[2] * ((double) yt[0] - (double) yt[1]));
cal_A = temp1 / temp2;
(*calibr).cali_A = (int32_t) ((double)cal_A * RESCALE_FACTOR);

//B
temp1 = (cal_A * ((double) xt[2] - (double) xt[1])) + (double) xd[1] - (double) xd[2];
temp2 = (double) yt[1] - (double) yt[2];
cal_B = temp1 / temp2;
(*calibr).cali_B = (int32_t) ((double)cal_B * RESCALE_FACTOR);

//C
cal_C = (double) xd[2] - (cal_A * (double) xt[2]) - (cal_B * (double) yt[2]);
(*calibr).cali_C = (int32_t) (cal_C * RESCALE_FACTOR);

//D
temp1 = ((double) yd[0] * ((double) yt[1] - (double) yt[2])) + ((double) yd[1] * ((double) yt[2] - (double) yt[0])) + ((double) yd[2] * ((double) yt[0] - (double) yt[1]));
temp2 = ((double) xt[0] * ((double) yt[1] - (double) yt[2])) + ((double) xt[1] * ((double) yt[2] - (double) yt[0])) + ((double) xt[2] * ((double) yt[0] - (double) yt[1]));
cal_D = (double)temp1 / (double)temp2;
(*calibr).cali_D = (int32_t) (cal_D * RESCALE_FACTOR);

//E
temp1 = (cal_D * ((double) xt[2] - (double) xt[1])) + (double) yd[1] - (double) yd[2];
temp2 = (double) yt[1] - (double) yt[2];
cal_E = (double)temp1 / (double)temp2;
(*calibr).cali_E = (int32_t) (cal_E * RESCALE_FACTOR);

//F
cal_F = (double) yd[2] - cal_D * (double) xt[2]  - cal_E * (double) yt[2] ;
(*calibr).cali_F = (int32_t) (cal_F * RESCALE_FACTOR);

LCD_WriteChar_8x16(11, 40, 'A',  RGB (0,0,0), RGB (255,255,0));
Lcd_UInt32_Out ((*calibr).cali_A, 20, 40, RGB (0,0,0), RGB (255,255,0));
LCD_WriteChar_8x16(11, 60, 'B',  RGB (0,0,0), RGB (255,255,0));
Lcd_UInt32_Out ((*calibr).cali_B, 20, 60, RGB (0,0,0), RGB (0,255,0));
LCD_WriteChar_8x16(11, 80, 'C',  RGB (0,0,0), RGB (255,255,0));
Lcd_UInt32_Out ((*calibr).cali_C, 20, 80, RGB (0,0,0), RGB (255,255,0));
LCD_WriteChar_8x16(11, 100, 'D',  RGB (0,0,0), RGB (255,255,0));
Lcd_UInt32_Out ((*calibr).cali_D, 20, 100, RGB (0,0,0), RGB (0,255,0));
LCD_WriteChar_8x16(11, 120, 'E',  RGB (0,0,0), RGB (255,255,0));
Lcd_UInt32_Out ((*calibr).cali_E, 20, 120, RGB (0,0,0), RGB (255,255,0));
LCD_WriteChar_8x16(11, 140, 'F',  RGB (0,0,0), RGB (255,255,0));
Lcd_UInt32_Out ((*calibr).cali_F, 20, 140, RGB (0,0,0), RGB (0,255,0));
}
