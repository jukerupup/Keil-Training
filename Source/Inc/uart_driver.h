#include "stm32f10x.h"
#define USART_BAUDRATE 115200
uint16_t USART_CalcBRR(uint32_t PCLK, uint32_t BaudRate);
void Uart1Config(void);
void USART1_SendChar(char c);
char USART1_ReadChar(void);
