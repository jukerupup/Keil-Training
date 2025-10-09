#include "stm32f10x.h"

void TIM2_Config(void)
{
	RCC->AHBENR |= (1<<0);
	TIM2->ARR=0xffff-1;
	TIM2->PSC=72-1;
}
