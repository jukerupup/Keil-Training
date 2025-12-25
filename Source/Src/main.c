#include "stm32f10x.h"
#include "Delay_F103.h"
#include "uart_driver.h"
#include <stdio.h>
#include "debug_log.h"
#include "FreeRTOS.h"
#include "task.h"

#define LOG_MODULE "main"
void SysClockConfig(void)
{
	/* RCC->CR |= RCC_CR_HSERDY;
	while (!(RCC->CR & RCC_CR_HSERDY));
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_0;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	RCC->CFGR |= RCC_CFGR_SW_HSI; */
	RCC->CR |= RCC_CR_HSEON; // 開啟 HSE
	while (!(RCC->CR & RCC_CR_HSERDY))
		; // 等待 HSE ready

	FLASH->ACR |= FLASH_ACR_LATENCY_2; // Flash latency 2 等級

	RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9; // PLL = HSE * 9
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY))
		; // 等待 PLL ready

	RCC->CFGR |= RCC_CFGR_SW_PLL; // SYSCLK = PLL
}

void GPIO_Config(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH &= ~(0xF << 20);
	GPIOC->CRH |= 1 << 20;
}

void vTask1(void *pvParameters)
{
	while (1)
	{
		GPIOC->ODR ^= (1 << 13); // toggle LED
		LOGE("Task1 hi\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
	// 可以在這裡打斷點或 LOG
	while (1);
}

int main(void)
{
	RCC->APB1ENR &= ~RCC_APB1ENR_WWDGEN; // 關閉時脈
	WWDG->CR = 0;
	WWDG->CFR = 0;
	WWDG->SR = 0;
	SysClockConfig();
	// SystemInit();
	GPIO_Config();
	// TIM2_Config();
	Uart1Config();
	// 停用 Window Watchdog（防止進入死迴圈）
	/* while (1)
	{
		Delay_ms(1000);
		//GPIOC->BSRR = (1 << (13 + 16));
		Delay_ms(1000);
		//GPIOC->BSRR = (1 << 13);
		//USART1_SendChar('C');
		//printf("%s, %f\n",str,a);
		LOGE("main error test\n");
		GPIOC->ODR ^= (1<<13); // toggle LED
	} */
	if (xTaskCreate(vTask1, "T1", 128, NULL, 1, NULL) != pdPASS)
	{
		LOGE("main error test\n");
	}
	vTaskStartScheduler();
	//while (1);
}
