
#define LCD_LED_ON (GPIOD->BSRR =GPIO_BSRR_BS13)
#define LCD_LED_OFF (GPIOD->BSRR =GPIO_BSRR_BR13)
#define LCD_RESET_OFF (GPIOE->BSRR =GPIO_BSRR_BS1)
#define LCD_RESET_ON (GPIOE->BSRR =GPIO_BSRR_BR1)
#define LCD_CS_OFF (GPIOD->BSRR =GPIO_BSRR_BS7)
#define LCD_CS_ON (GPIOD->BSRR =GPIO_BSRR_BR7)
#define LCD_RS_1 (GPIOD->BSRR =GPIO_BSRR_BS11)	//RS 1
#define LCD_RS_0 (GPIOD->BSRR =GPIO_BSRR_BR11)//RS 0
#define LCD_WR_OFF (GPIOD->BSRR =GPIO_BSRR_BS5)
#define LCD_WR_ON (GPIOD->BSRR =GPIO_BSRR_BR5)
#define LCD_OE_OFF (GPIOD->BSRR =GPIO_BSRR_BS4)
#define LCD_OE_ON (GPIOD->BSRR =GPIO_BSRR_BR4)
#define LED_2_ON (GPIOD->BSRR =GPIO_BSRR_BS3)
#define LED_2_OFF (GPIOD->BSRR =GPIO_BSRR_BR3)

//colors:
#define RGB(r,g,b)  ((uint16_t)((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))
#define BLACK 0x0000
#define GREEN 0x0F00
#define YELLOW 0xFF00
#define RED 0xF000
#define BLUE 0x00FF

void Lcd_Reset (void);
void Lcd_Set (int com);
void Lcd_Com (int com);
void Lcd_Data (int data);
void Lcd_Com_Data (int addr, int data);
void Lcd_Port_In_Conf (void);
int Lcd_Read (void);
void InitLcd (void);

