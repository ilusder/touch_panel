#include "lcd_ports.h"

void Lcd_Reset (void)
{
LCD_RESET_ON;
Delay (300);
LCD_RESET_OFF;
}

void Lcd_Set (int com)
{
int tmp;
GPIOE->BRR = 0xFF80;  //clear port data bits only
GPIOD->BRR = 0xC703;	 //clean port  data bits only
tmp = com  & 0x1FF0;  //mask for Port E;
tmp = tmp << 3;     //move data to PE15 - PE7
GPIOE->BSRR = tmp;	 //out on port E
tmp = com & 0xE000;    //Mask for port D (hi part) 
tmp = tmp >> 5;
GPIOD->BSRR = tmp;
tmp = com & 0x000C;    //Mask for port D (LO part)
tmp = tmp >> 2;
GPIOD->BSRR = tmp;
tmp = com & 0x0003;    //Mask for port D (LO part)
tmp = tmp << 14;
GPIOD->BSRR = tmp;
}

void Lcd_Com (int com)
{
Lcd_Set (com);
LCD_OE_OFF;
LCD_RS_0;	   //addr set
LCD_WR_ON;	   //Write com
Delay (1);
LCD_WR_OFF;
}

void Lcd_Data (int data)
{
Lcd_Set (data);
LCD_OE_OFF;
LCD_RS_1;	   //data sel
LCD_WR_ON;	   //Write data
Delay (1);
LCD_WR_OFF;
}

void Lcd_Com_Data (int addr, int data)
{
LCD_CS_ON;
Lcd_Com	(addr);
Lcd_Data	(data);
LCD_CS_OFF;
Delay (2);
}

void Lcd_Port_In_Conf (void)
{
// Конфигурируем CRL регистры LCD.
GPIOD->CRH = 0x44222444;		// Конфиг ввода порта D (Hi) - Bit 
GPIOD->CRL = 0x22222444;			// Конфиг ввода порта D (Low)  Bit 4,5,7  - outs

GPIOE->CRH = 0x44444444;		// Конфиг ввода порта E (Hi)
GPIOE->CRL = 0x42222222;		// Конфиг ввода порта E (Low) pin 1 - Reset LCD
}

void Lcd_Port_Out_Conf (void)
{
// Конфигурируем CRL регистры LCD.
GPIOD->CRH = 0x22222222;		// Конфиг выводов порта D (Hi) - ALL BITS in Use
GPIOD->CRL = 0x24222222;			// Конфиг выводов порта D (Low) - In Use Bits 7 5 4 1 0;

GPIOE->CRH	= 0x22222222;		// Конфиг выводов порта E (Hi)
GPIOE->CRL	= 0x24444424;		// Конфиг выводов порта E (Low) - In Use Bits 7 1;
}

int Lcd_Read (void)
{
int tmp1, tmp2;
int data=0, tmp;
Lcd_Com (0x0000);
LCD_CS_ON;
Delay (5);
Lcd_Port_In_Conf ();   //config ports as Input
LCD_RS_1;
LCD_OE_ON;
Delay (5);
tmp1 = GPIOE->IDR;  //get Port E data
tmp2 = GPIOD->IDR;  //get Port D data
Delay (5);
LCD_OE_OFF;
LCD_CS_OFF;
Lcd_Port_Out_Conf ();   //return ports as Out
//prepeare port E data
tmp1 &= 0xFF80;	  //mask for PE15 - PE7
tmp1 = tmp1 >> 3;	//to D12 - D4
data |= tmp1;       //Write to data
//prepeare port D data
tmp = tmp2 & 0x0700; //mask PD10 - PD8
tmp = tmp << 5;           //to D15 - D13
data |= tmp;        //Write
tmp = tmp2 & 0xC000; //mask PD15 - PD14
tmp = tmp >> 14;           //to D1 - D0
data |= tmp;        //Write
tmp = tmp2 & 0x0003; //mask PD1 - PD0
tmp = tmp << 2;           //to D3 - D2
data |= tmp;        //Write
return data;
}

void InitLcd (void)
{
RCC->APB2ENR 	|= RCC_APB2ENR_IOPDEN;	 //Порт D
RCC->APB2ENR 	|= RCC_APB2ENR_IOPEEN;	 //Порт E

Lcd_Port_Out_Conf ();	//config ports as Out
GPIOE->ODR = 0;  //clear port
GPIOD->ODR = 0;	 //clean port

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