#include "stm32f10x.h"
#include "Delay_F103.h"
#include "UART.h"

uint16_t USART_CalcBRR(uint32_t PCLK, uint32_t BaudRate)
{
	float USARTDIV = (float)PCLK / (16.0f * (float)BaudRate);
	int Mantissa = (int)USARTDIV;

	float frac = (USARTDIV - Mantissa) * 16.0f;
	int Fraction = (int)(frac + 0.5f);   // Rounding

	if (Fraction >= 16)
	{
		Mantissa += 1;
		Fraction = 0;
	}

	return (Mantissa << 4) | Fraction;
}

void Uart1Config(void)
{
	uint16_t brr;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10 | GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
	// Set PA9 be AF PP, 50MHz
	GPIOA->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9);	//1011b = CNF=10, MODE=11
	// Set PA10 be Input floating
	GPIOA->CRH |= GPIO_CRH_CNF10_0;  // 0100b = CNF=01, MODE=00
	brr=USART_CalcBRR(GetHCLKFreq(), USART_BAUDRATE);
	USART1->BRR = brr;
	USART1->CR1 = 0;                // Reset
	USART1->CR1 |= ((USART_CR1_UE) | (USART_CR1_TE) | (USART_CR1_RE));    // UE: Enable USART, TE: Enable Tx, RE: Enable Rx
}

void USART1_SendChar(char c) 
{
	while (!(USART1->SR & (1 << 7))); // TXE
	USART1->DR = c;
}

char USART1_ReadChar(void) 
{
	while (!(USART1->SR & (1 << 5))); // RXNE
	return USART1->DR;
}
