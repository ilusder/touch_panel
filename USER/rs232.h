/*-------------------------
-------  RS232 interface --
---------Ilya Deryabin-----
-------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void serial2_setup(void);
unsigned char USART2_receive(void);
void USART2_send(unsigned char value);
void USART2_Send_Str (char *s);
