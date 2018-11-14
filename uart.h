#ifndef UART_H_
#define UART_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD 9600

#include <stdio.h>

void uartInit(void);
int uartPutchar(char c, FILE *stream);
int uartGetchar(FILE *stream);

#endif /* UART_H_ */