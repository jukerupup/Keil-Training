#include "stm32f10x.h"

void TIM2_Config(void)
{
	RCC->AHBENR |= (1<<0);
	TIM2->ARR=0xffff-1;
	TIM2->PSC=72-1;
}

uint32_t GetHCLKFreq(void)
{
	uint32_t sysclk = 0;
	uint32_t pllmul, pllsrc;
	uint32_t current_sysclk_src, hpre;

	/*---------------------------------------------------------
	* 1. Read system clock source (SWS bits in RCC_CFGR[3:2])
	*    00: HSI
	*    01: HSE
	*    10: PLL
	*--------------------------------------------------------*/
	current_sysclk_src = (RCC->CFGR >> 2) & 0x3;		// Read System Clock Switch Status bits (SWS[1:0])

	if (RCC_CFGR_SW_HSI == current_sysclk_src)			// HSI used as system clock
	{
		sysclk = HSI_VALUE;								// HSI = 8 MHz internal RC
	}
	else if (RCC_CFGR_SW_HSE == current_sysclk_src)		// HSE used as system clock
	{
		sysclk = HSE_VALUE;  // Assume external crystal = 8 MHz
	}
	else if (RCC_CFGR_SW_PLL == current_sysclk_src)		// PLL used as system clock
	{
		/*---------------------------------------------------------
		* 2. Read PLL configuration
		*    - PLLMUL[21:18]: multiplier factor (x2 ... x16)
		*    - PLLSRC[16]: 0 = HSI/2, 1 = HSE
		*--------------------------------------------------------*/
		pllmul = ((RCC->CFGR >> 18) & 0xF) + 2;			// actual multiplier = value + 2
		pllsrc = (RCC->CFGR >> 16) & 0x1;

		if (pllsrc == 0)
		{
			sysclk = (HSI_VALUE / 2) * pllmul;			// PLL source = HSI/2
		}
		else
		{
			sysclk = HSE_VALUE * pllmul;				// PLL source = HSE
		}
	}

	/*---------------------------------------------------------
	* 3. Read AHB prescaler (HPRE[7:4])
	*    0000: SYSCLK / 1
	*    1000: SYSCLK / 2
	*    1001: SYSCLK / 4
	*    1010: SYSCLK / 8
	*    1011: SYSCLK / 16
	*    1100: SYSCLK / 64
	*    1101: SYSCLK / 128
	*    1110: SYSCLK / 256
	*    1111: SYSCLK / 512
	*--------------------------------------------------------*/
	hpre = (RCC->CFGR >> 4) & 0xF;

	switch (hpre)
	{
		case 0x0 ... 0x7:	return sysclk;				// No division
		case 0x8:			return sysclk / 2;
		case 0x9:			return sysclk / 4;
		case 0xA:			return sysclk / 8;
		case 0xB:			return sysclk / 16;
		case 0xC:			return sysclk / 64;
		case 0xD:			return sysclk / 128;
		case 0xE:			return sysclk / 256;
		case 0xF:			return sysclk / 512;
		default:			return sysclk;
		}
}