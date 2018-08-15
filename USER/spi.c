/*------------------------
-------  SPI interface ---
---------Ilya Deryabin----
------------------------*/

#include "spi.h"
#include "delay.h"

void spi_touch_conf (void)
{
//ports SPI (A and B config)
RCC->APB2ENR 	|= RCC_APB2ENR_IOPAEN;	 //Порт A
RCC->APB2ENR 	|= RCC_APB2ENR_IOPBEN;	 //Порт B
//in-out config
//PA5 - SCL	- Alternative func. push-pull	  (A)
//PB7 - CS - GPIO - soft	(4)
//PA6        - MOSI	 Alternative func.  push-pull- OUT  Alternate function push-pull   (A)
//PA7	    - MISO Input floating / Input pull-up - IN
//PB6 - Interupt	 - input
GPIOA->CRL = 0xA4A44444;	// Port A (Low)  Bit 5,6  - alt func	7 - input
//GPIOA->CRL &= ~GPIO_CRL_MODE6; //PA6(MISO) - Input
//GPIOA->CRL |= GPIO_CRL_CNF6_0; //PA6(MISO) - Input floating
//GPIOA->CRL |= GPIO_CRL_CNF7_1 | GPIO_CRL_CNF5_1; //PA7(MOSI), PA5(SCK)
//GPIOA->CRL |= GPIO_CRL_MODE7 | GPIO_CRL_MODE5; // MOSI, SCK

GPIOB->CRL = 0x24444444;		// Port B (Low)	bit 7 - out, bit 6 - interupt in

//SPI1 CR1 configure
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //Enable oscil SPI1
  SPI1->CR1 |= SPI_CR1_BR; //Baud rate = Fpclk/256
  SPI1->CR1 &= ~SPI_CR1_CPOL; //Polarity cls sygnal	CPOL=0;
  SPI1->CR1 &= ~SPI_CR1_CPHA; //Phase cls sygnal    CPHA = 0;
  SPI1->CR1 &= ~SPI_CR1_DFF; //8 bit data
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST; //MSB will be first
  SPI1->CR1 |= SPI_CR1_SSM; // Programm mode NSS
  SPI1->CR1 |= SPI_CR1_SSI; // Анналогично состоянию, когда на NSS высокий уравень
  SPI1->CR2 |= SPI_CR2_SSOE; //вывод NSS - вывод состояния slave select
  SPI1->CR1 |= SPI_CR1_MSTR; //Mode Master
  SPI1->CR1 |= SPI_CR1_SPE; //Enable SPI1
//SPI1->CR1 = 0x0000087C;  //Master enable 8 bit data No CRC; PCK/256; from 0 to 1; CPOL=0; CPHA = 0;

//--------------------------
SPI1->CR2 = 0x00000000;  //No interupt enable
}

int get_touch (uint8_t sir)	 //comunication with TSC2046 - touch screen controller
{
int d_in;		 //zero data in
CS_TOUCH_ON;	 //chip select TOUCH
Delay (10);
SPI1->DR = sir;	  //send data
while(!(SPI1->SR & SPI_SR_RXNE));	//wait
d_in = SPI1->DR;				  //ignore this enter
SPI1->DR = 0;	   //send zero bytes - cont recieving data from module
while(!(SPI1->SR & SPI_SR_RXNE));  //wait
d_in = SPI1->DR;	//recive hight byte
d_in <<= 8;			//move it
SPI1->DR = 0;		  //send zero bytes - cont recieving data from module
while(!(SPI1->SR & SPI_SR_RXNE));	//wait
d_in |= SPI1->DR;	 //recive low byte
CS_TOUCH_OFF;
Delay (10);
return d_in;
}
