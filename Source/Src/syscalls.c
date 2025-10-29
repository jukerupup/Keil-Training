/**
 ******************************************************************************
 * @file      syscalls.c
 * @brief     STM32CubeIDE Minimal System calls file
 ******************************************************************************
 */

#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <UART.h>

void __io_putchar(char ch)
{
    USART1_SendChar(ch);
}

int __io_getchar(void)
{
    return USART1_ReadChar();
}

int _write(int file, char *ptr, int len)
{
  for (int i = 0; i < len; i++)
    __io_putchar(*ptr++);
  return len;
}

int _read(int file, char *ptr, int len)
{
  for (int i = 0; i < len; i++)
    *ptr++ = __io_getchar();
  return len;
}

int _close(int file)
{
  return -1;
}

int _fstat(int file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file)
{
  return 1;
}

int _lseek(int file, int ptr, int dir)
{
  return 0;
}

void *_sbrk(ptrdiff_t incr)
{
  extern char _end; // Defined by the linker script
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0)
    heap_end = &_end;

  prev_heap_end = heap_end;
  heap_end += incr;

  return (void *)prev_heap_end;
}
