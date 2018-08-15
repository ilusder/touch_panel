#include "stm32f10x.h"
#include "Lcd_Graph.h"
#include "spi.h"
#include "delay.h"
#include "rs232.h"

//point 1: x - 50%; y - 10%
//point 2: x - 90%; y - 50%
//point 3: x - 10%; y - 90%
//----------------------------------------
//|    3                                  |
//|                                       |
//|                                       |
//|                                  1    |
//|                                       |
//|                                       |
//|                   2                   |
//----------------------------------------

/*-------------------
-----Touch screen----
-------------------*/

#define TOUCH_X_MAX 240
#define TOUCH_Y_MAX 320
#define TOUCH_MIN_X 0
#define TOUCH_MIN_Y 0

#define RESCALE_FACTOR 1000000
#define DIV_TOUCH 1

#define ADC_X_MIN 6000
#define ADC_X_MAX 31000
#define ADC_Y_MIN 5500
#define ADC_Y_MAX 31500
#define ADC_X_APP_MAX (ADC_X_MAX / DIV_TOUCH)	 //approach X max
#define ADC_Y_APP_MAX (ADC_Y_MAX / DIV_TOUCH)	 //approach Y max
 
#define GET_X 0x98		//TSC2043 param	  0x98
#define GET_Y 0xD8		//TSC2043 param	  0xD8

struct calibration
{
volatile int32_t cali_A;
volatile int32_t cali_B;
volatile int32_t cali_C;
volatile int32_t cali_D;
volatile int32_t cali_E;
volatile int32_t cali_F;
};

struct point 
{
volatile int32_t d_in_x;
volatile int32_t d_in_y;
};


void spi_touch_chat (uint8_t i, struct point *xypoint);
void touch_correct (struct point *xypoint,struct calibration *calibr);
void Touch_Calib (struct calibration *calibr);
