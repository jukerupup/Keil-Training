#include "stm32f10x.h"
#include "Delay_F103.h"
#include "UART.h"

void SysClockConfig(void)
{
	RCC->CR |= RCC_CR_HSERDY;
	while (!(RCC->CR & RCC_CR_HSERDY));
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_0;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
}

void GPIO_Config(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH &= ~(0xF << 20);
	GPIOC->CRH |= 1 << 20;
}

int main(void)
{
	SysClockConfig();
	// SystemInit();
	GPIO_Config();
	TIM2_Config();
	Uart1Config();
	while (1)
	{
		Delay_ms(1000);
		GPIOC->BSRR = (1 << (13 + 16));
		Delay_ms(1000);
		GPIOC->BSRR = (1 << 13);
		USART1_SendChar('C');
	}
}
