#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_rcc.h"

#define LCD_DATA ((uint32_t)0x60020000)    
#define LCD_REG  ((uint32_t)0x60000000)

#define LCD_LED_ON (GPIOD->BSRR =GPIO_BSRR_BS13)
#define LCD_LED_OFF (GPIOD->BSRR =GPIO_BSRR_BR13)
#define LCD_RESET_OFF (GPIOE->BSRR =GPIO_BSRR_BS1)
#define LCD_RESET_ON (GPIOE->BSRR =GPIO_BSRR_BR1)
#define LED_2_ON (GPIOD->BSRR =GPIO_BSRR_BS3)
#define LED_2_OFF (GPIOD->BSRR =GPIO_BSRR_BR3)


void Lcd_Reset (void);
void Lcd_Com (int com);
void Lcd_Data (int data);
void Lcd_Com_Data (int addr, int data);
int Lcd_Read (void);
void LCD_FSMCConfig(void);
void Lcd_Port_Conf (void);
void InitLcd (void);
