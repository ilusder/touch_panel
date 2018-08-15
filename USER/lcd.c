
#define LCD_DATA ((uint32_t)0x60020000)    

#define LCD_REG  ((uint32_t)0x60000000)

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


void Lcd_Reset (void)
{
LCD_RESET_ON;
Delay (300);
LCD_RESET_OFF;
}

void Lcd_Com (int com)
{
 *(uint16_t *) (LCD_REG) = com;	
}

void Lcd_Data (int data)
{
*(uint16_t *) (LCD_DATA)= data;
}

void Lcd_Com_Data (int addr, int data)
{
LCD_CS_ON;
Lcd_Com	(addr);
Lcd_Data (data);
LCD_CS_OFF;
Delay (2);
}

void LCD_FSMCConfig(void)
{

}

void Lcd_Port_Conf (void)
{
// Конфигурируем CRH регистры LCD. Port D
//RS	->	PD11
//DB0	->	PD14
//DB1	->	PD15
//DB13	->	PD8
//DB14	->	PD9
//DB15	->	PD10
//Light ->	PD13 - 0010	 - 2 - Output push pull 10 Mhz
GPIOD->CRH	= 0xBB24BBBB;		//8 - Alternative function out - push-pull mode out	50 Mhz
// Конфигурируем CRL регистры LCD. Port D
//WR		->	PD5
//RD		->	PD4
//CS		->	PD7
//DB2		->	PD0
//DB3		->	PD1
GPIOD->CRL	=  0xB4BB44BB;	   //8 - Alternative function out - push-pull mode out	50 Mhz
// Конфигурируем CRH регистры LCD. Port E
//DB5		->	PE8
//DB6		->	PE9
//DB8		->	PE11
//DB9		->	PE12
//DB10		->	PE13
//DB11		->	PE14
//DB12		->	PE15
GPIOE->CRH	=  0xBBBBBBBB; 	   //B - Alternative function out - push-pull mode out	50 Mhz
// Конфигурируем CRL регистры LCD. Port E
//DB4		->	PE7
//RESET		->	PE1		- 0010	 - 2 - Output push pull 10 Mhz
GPIOE->CRL	=  0xB4444424;	   //B - Alternative function out - push-pull mode out	50 Mhz
}

int Lcd_Read (void)
{
int tmp1;
*(uint16_t *) (LCD_REG) = 0x0000;
tmp1 |= *(uint16_t *) LCD_DATA;        //Read
return tmp1;
}

void InitLcd (void)
{
RCC->APB2ENR 	|= RCC_APB2ENR_IOPDEN;	 //Порт D
RCC->APB2ENR 	|= RCC_APB2ENR_IOPEEN;	 //Порт E

RCC->AHBENR |= RCC_AHBENR_FSMCEN; //Enable oscil FSMC
Lcd_Port_Conf ();
LCD_FSMCConfig();	//config FSMC


Lcd_Reset ();
LCD_LED_ON;
if  (Lcd_Read () == 0x9325)
	{	
	GPIOB->BSRR =GPIO_BSRR_BS5;		// Set 
	Delay(100);
	GPIOB->BSRR =GPIO_BSRR_BR5;		// Res
	Lcd_Com_Data (0x00e7,0x0010); 
	LED_2_ON; 
    Lcd_Com_Data (0x0000,0x0001);       /* start internal osc */
    Lcd_Com_Data (0x0001,0x0100);     
    Lcd_Com_Data (0x0002,0x0700);       /* power on sequence */
    Lcd_Com_Data (0x0003,(1<<12)|(0<<5)|(1<<4)|(0<<3) );        /* importance */
    Lcd_Com_Data (0x0004,0x0000);                                   
    Lcd_Com_Data (0x0008,0x0207);                  
    Lcd_Com_Data (0x0009,0x0000);         
    Lcd_Com_Data (0x000a,0x0000);       /* display setting */        
    Lcd_Com_Data (0x000c,0x0001);       /* display setting */        
    Lcd_Com_Data (0x000d,0x0000);                               
    Lcd_Com_Data (0x000f,0x0000);     /* Power On sequence */
    Lcd_Com_Data (0x0010,0x0000);   
    Lcd_Com_Data (0x0011,0x0007);
    Lcd_Com_Data (0x0012,0x0000);                                                                 
    Lcd_Com_Data (0x0013,0x0000);  Delay (2000);  /* delay 50 ms */             
    Lcd_Com_Data (0x0010,0x1590);   
    Lcd_Com_Data (0x0011,0x0227);  Delay (2000);  /* delay 50 ms */        
    Lcd_Com_Data (0x0012,0x009c);  Delay (2000);  /* delay 50 ms */             
    Lcd_Com_Data (0x0013,0x1900);   
    Lcd_Com_Data (0x0029,0x0023);
    Lcd_Com_Data (0x002b,0x000e);  Delay (2000);  /* delay 50 ms */             
    Lcd_Com_Data (0x0020,0x0000);                                                            
    Lcd_Com_Data (0x0021,0x0000);  Delay (2000);  /* delay 50 ms */             
    Lcd_Com_Data (0x0030,0x0007); 
    Lcd_Com_Data (0x0031,0x0707);   
    Lcd_Com_Data (0x0032,0x0006);
    Lcd_Com_Data (0x0035,0x0704);
    Lcd_Com_Data (0x0036,0x1f04); 
    Lcd_Com_Data (0x0037,0x0004);
    Lcd_Com_Data (0x0038,0x0000);        
    Lcd_Com_Data (0x0039,0x0706);     
    Lcd_Com_Data (0x003c,0x0701);
    Lcd_Com_Data (0x003d,0x000f);  Delay (2000);  /* delay 50 ms */             
    Lcd_Com_Data (0x0050,0x0000);        
    Lcd_Com_Data (0x0051,0x00ef);   
    Lcd_Com_Data (0x0052,0x0000);     
    Lcd_Com_Data (0x0053,0x013f);
    Lcd_Com_Data (0x0060,0xa700);        
    Lcd_Com_Data (0x0061,0x0001); 
    Lcd_Com_Data (0x006a,0x0000);
    Lcd_Com_Data (0x0080,0x0000);
    Lcd_Com_Data (0x0081,0x0000);
    Lcd_Com_Data (0x0082,0x0000);
    Lcd_Com_Data (0x0083,0x0000);
    Lcd_Com_Data (0x0084,0x0000);
    Lcd_Com_Data (0x0085,0x0000);
    Lcd_Com_Data (0x0090,0x0010);     
    Lcd_Com_Data (0x0092,0x0000);  
    Lcd_Com_Data (0x0093,0x0003);
    Lcd_Com_Data (0x0095,0x0110);
    Lcd_Com_Data (0x0097,0x0000);        
    Lcd_Com_Data (0x0098,0x0000);  /* display on sequence */    
    Lcd_Com_Data (0x0007,0x0133);	 //Power On
    Lcd_Com_Data (0x0020,0x0000);                                                            
    Lcd_Com_Data (0x0021,0x0000);
	LED_2_OFF;
	}
  //-----------------------------------------------------------------------------
  //                                  SSD1289
  //----------------------------------------------------------------------------- 
if( Lcd_Read () == 0x8989 )
  {
    GPIOB->BSRR =GPIO_BSRR_BS5;		// Set 
	Delay(100000);
	GPIOB->BSRR =GPIO_BSRR_BR5;		// Res
	Lcd_Com_Data (0x00e7,0x0010); 
	LED_2_ON; 
    Lcd_Com_Data (0x0000,0x0001);    Delay(50);   
    Lcd_Com_Data (0x0003,0xA8A4);    Delay(50);   
    Lcd_Com_Data(0x000C,0x0000);    Delay(50);   
    Lcd_Com_Data(0x000D,0x080C);    Delay(50);   
    Lcd_Com_Data(0x000E,0x2B00);    Delay(50);   
    Lcd_Com_Data(0x001E,0x00B0);    Delay(50);   
    Lcd_Com_Data(0x0001,0x2B3F);    Delay(50);   /* 320*240 0x2B3F */
    Lcd_Com_Data(0x0002,0x0600);    Delay(50);
    Lcd_Com_Data(0x0010,0x0000);    Delay(50);
    Lcd_Com_Data(0x0011,0x6070);    Delay(50);   
    Lcd_Com_Data(0x0005,0x0000);    Delay(50);
    Lcd_Com_Data(0x0006,0x0000);    Delay(50);
    Lcd_Com_Data(0x0016,0xEF1C);    Delay(50);
    Lcd_Com_Data(0x0017,0x0003);    Delay(50);
    Lcd_Com_Data(0x0007,0x0133);    Delay(50);         
    Lcd_Com_Data(0x000B,0x0000);    Delay(50);
    Lcd_Com_Data(0x000F,0x0000);    Delay(50);   
    Lcd_Com_Data(0x0041,0x0000);    Delay(50);
    Lcd_Com_Data(0x0042,0x0000);    Delay(50);
    Lcd_Com_Data(0x0048,0x0000);    Delay(50);
    Lcd_Com_Data(0x0049,0x013F);    Delay(50);
    Lcd_Com_Data(0x004A,0x0000);    Delay(50);
    Lcd_Com_Data(0x004B,0x0000);    Delay(50);
    Lcd_Com_Data(0x0044,0xEF00);    Delay(50);
    Lcd_Com_Data(0x0045,0x0000);    Delay(50);
    Lcd_Com_Data(0x0046,0x013F);    Delay(50);
    Lcd_Com_Data(0x0030,0x0707);    Delay(50);
    Lcd_Com_Data(0x0031,0x0204);    Delay(50);
    Lcd_Com_Data(0x0032,0x0204);    Delay(50);
    Lcd_Com_Data(0x0033,0x0502);    Delay(50);
    Lcd_Com_Data(0x0034,0x0507);    Delay(50);
    Lcd_Com_Data(0x0035,0x0204);    Delay(50);
    Lcd_Com_Data(0x0036,0x0204);    Delay(50);
    Lcd_Com_Data(0x0037,0x0502);    Delay(50);
    Lcd_Com_Data(0x003A,0x0302);    Delay(50);
    Lcd_Com_Data(0x003B,0x0302);    Delay(50);
    Lcd_Com_Data(0x0023,0x0000);    Delay(50);
    Lcd_Com_Data(0x0024,0x0000);    Delay(50);
    Lcd_Com_Data(0x0025,0x8000);    Delay(50);
    Lcd_Com_Data(0x004f,0);        
    Lcd_Com_Data(0x004e,0);        
  }  
}

