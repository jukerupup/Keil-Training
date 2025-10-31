#include "stm32f10x.h"
#include "Delay_F103.h"
#include "uart_driver.h"
#include <stdio.h>

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
	char *str="aabbccdd";
	float a=1.4;
	SysClockConfig();
	// SystemInit();
	GPIO_Config();
	TIM2_Config();
	Uart1Config();
	// 停用 Window Watchdog（防止進入死迴圈）
	RCC->APB1ENR &= ~RCC_APB1ENR_WWDGEN;// 關閉時脈
	WWDG->CR = 0;
	WWDG->CFR = 0;
	WWDG->SR = 0;
	while (1)
	{
		Delay_ms(1000);
		GPIOC->BSRR = (1 << (13 + 16));
		Delay_ms(1000);
		GPIOC->BSRR = (1 << 13);
		//USART1_SendChar('C');
		printf("%s, %f\n",str,a);
	}
}
