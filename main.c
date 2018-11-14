#include "./others/def_principais.h" //inclusão do arquivo com as principais definições
#include "./LCD/LCD.h"
#include "./medTemp/medTemp.h"
#include "./others/uart.h"
#include <stdlib.h>
#include <string.h>

#ifndef F_CPU
#define F_CPU 16000000UL //define a frequência do microcontrolador - 16MHz
#endif

const char temperatura[] PROGMEM = "Temper.: "; //mensagem armazenada na memória flash
const char umidade[] PROGMEM = "Umidade: "; //mensagem armazenada na memória flash

const uint8_t posicDisplay[2][16] PROGMEM = {
    {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},
    {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF}
};
//-----------------------------------------------------------------------------------
/*
VSS   = GND
VDD   = VCC (5V)
V0    = GND
RS    = 8 (PB0)
RW    = GND
E     = 9 (PB1)
D0-D3 = -
D4-D7 = 4-7 (PD4-PD7)
A     =	VCC (5V)
K     = GND
*/

uint8_t getPos(uint8_t linha, uint8_t coluna){
    return pgm_read_byte(&posicDisplay[linha][coluna]);
}

char* integer_to_string(int x){
    char* buffer = malloc(sizeof(char) * sizeof(int) * 4 + 1);
    if (buffer)
    {
         sprintf(buffer, "%d", x);
    }
    return buffer; // caller is expected to invoke free() on this buffer to release memory
}

int main()
{
    //uint8_t sensor = PD3;
    //uint8_t vec[] = {0, 0, 0, 0, 0};
    char *temp;
    char *umid;
    char *aux;

    uartInit();
    //startTempAndUmidSen();
    inic_LCD_4bits(); //inicializa o LCD
    DDRD = 0xFF; //PORTD como saída
    DDRB = 0xFF; //PORTB como saída
 
    escreve_LCD_Flash(temperatura);
    cmd_LCD(0xC0,0); // move o cursor pra segunda linha
    escreve_LCD_Flash(umidade);

    while(1){
        
    }
}
//================================================================================ //
