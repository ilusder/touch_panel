/*------------------------
-------  SPI interface ---
---------Ilya Deryabin----
------------------------*/
#include "stm32f10x.h"

#define CS_TOUCH_ON   (GPIOB->BSRR =GPIO_BSRR_BR7)			//negative logic
#define CS_TOUCH_OFF  (GPIOB->BSRR =GPIO_BSRR_BS7)			//negative logic

//-------------

void spi_touch_conf (void);
int get_touch (uint8_t sir); //comunication with TSC2046 - touch screen controller


