#include "rs232.h"

void serial2_setup()

{
//UART INT ON
NVIC_EnableIRQ (USART1_IRQn);
__enable_irq ();

// PORT INIT	
RCC->APB2ENR  |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN |	// GPIOA Clock ON. Alter function clock ON
		 RCC_APB2ENR_IOPBEN;				// GPIOB Clock ON

// USART2 Settings
// SET TX2
GPIOA->CRL	&= ~GPIO_CRL_CNF2;	// Clear CNF bit 2
GPIOA->CRL	|= GPIO_CRL_CNF2_1;	// Set CNF bit 2 to 10 - AFIO Push-Pull
GPIOA->CRL	|= GPIO_CRL_MODE2_0;	// Set MODE bit 2 to Mode 01 = 10MHz 
 
// SET RX2
GPIOA->CRL		&= ~GPIO_CRL_CNF3;	// Clear CNF bit 3
GPIOA->CRL		|= GPIO_CRL_CNF3_0;	// Set CNF bit 3 to 01 HiZ
GPIOA->CRL		&= ~GPIO_CRL_MODE3;	// Set MODE bit 3 to Mode 01 = 10MHz 
 
 
// SET USART2 REG	
RCC->APB1ENR	|= RCC_APB1ENR_USART2EN;				// USART2 Clock ON
USART1->BRR 	= 0xEA6;					// Bodrate for 19200 on 72Mhz
USART2->CR1 	|= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE |		// USART2 ON, TX ON, RX ON
USART_CR1_RXNEIE;	
}

void USART2_IRQHandler(void)
{
	if (USART2->SR & USART_SR_RXNE)
	{
	USART2->DR = (USART2->DR);
	}
}


void USART2_send(unsigned char value)
{
     while(USART2->SR & (USART_SR_TXE==0));                          //wait until data transmission is complete
     USART2->DR = value;                                                  //transfer data to DR register
}

void USART2_Send_Str (char *s)
	{
	while  (*s!='\0')
		{ 
		USART2_send (*s);
		s++;
		}
	}
