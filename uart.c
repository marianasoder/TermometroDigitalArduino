#include "uart.h"

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/setbaud.h>
#include <stdio.h>

static FILE uart_io = FDEV_SETUP_STREAM(uartPutchar, uartGetchar, _FDEV_SETUP_RW);

void uartInit(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
    UCSR0A |= _BV(U2X0);
    #else
    UCSR0A &= ~(_BV(U2X0));
    #endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */

    stdout = &uart_io;
    stdin = &uart_io;
}

int uartPutchar(char c, FILE *stream) {
    if (c == '\n') {
        uartPutchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

int uartGetchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}