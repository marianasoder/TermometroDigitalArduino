#include "def_principais.h" //inclusão do arquivo com as principais definições
#include "LCD.h"
#include "uart.h"
#include <stdlib.h>
#include <string.h>

#ifndef F_CPU
#define F_CPU 16000000UL //define a frequência do microcontrolador - 16MHz
#endif

const char temperatura[] PROGMEM = "Temp:  "; //mensagem armazenada na memória flash
const char umidade[] PROGMEM = "Umid:  "; //mensagem armazenada na memória flash

//-----------------------------------------------------------------------------------
/*
SENSOR DE TEMP/UMID:
PD3

LCD:

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

uint8_t getTempAndUmid(uint8_t* vec, uint8_t sensor){
    uint8_t i, j, cont;
    vec[0] = 0b00000000; // Parte inteira do RH
    vec[1] = 0b00000000; // Parte decimal do RH
    vec[2] = 0b00000000; // Parte inteira de T
    vec[3] = 0b00000000; // Parte decimal de T
    vec[4] = 0b00000000; // checksum

    set_bit(DDRD, sensor);  // Ativa o sensor como saida
    set_bit(PORTD, sensor); // deixa o nivel alto

    clr_bit(PORTD, sensor);                 // MCU manda o sinal
    _delay_ms(18);                          // Espera 18ms
    
    set_bit(PORTD, sensor);                 //ativa o Pull Up
    clr_bit(DDRD, sensor);

    while(tst_bit(PIND, sensor));           // Enquanto aguarda a resposta do DHT por 20-40us
    while(!tst_bit(PIND, sensor));          // DHT envia sinal de resposta por +- 80us 
    while(tst_bit(PIND, sensor));           // DHT ativa o Pull Up por +- 80us 

    for(i=0; i<5;i++){
        for(j=8; j>=1; j--){
            cont = 0;                       // Zera o contador
            while(!tst_bit(PIND, sensor));  // Espera ficar em nivel alto
            while(tst_bit(PIND, sensor)){   // quando ficar alto
                //_delay_us(1);             // conta o tempo que ficou alto
                cont++;
            }

            if(cont > 65){                  // Setar em 1
                set_bit(vec[i], j-1);
            }                               // caso contrario eh zero (como ja esta setado)
        }
    }
    // Retorna se o checkSum bateu
    return ((vec[0]+vec[1]+vec[2]+vec[3]) == vec[4]);
}

// Converte um inteiro em string
char* integer2string(int x){
    char* buffer = malloc(sizeof(char) * sizeof(int) * 4 + 1);
    if (buffer)
    {
         sprintf(buffer, "%d", x);
    }
    return buffer;
}

int main()
{
    uint8_t sensor = PD3;
    uint8_t vec[] = {0, 0, 0, 0, 0}; // Vetor para que o sensor escreva os valores
    char *temp;
    char *umid;
    uint8_t i,j; 

    uartInit();
    _delay_ms(2000); // Da 2s de Delay pra sensor inicializar
    DDRD = 0xFF; //PORTD como saída
    DDRB = 0xFF; //PORTB como saída
    inic_LCD_4bits(); //inicializa o LCD

    while(1){
        if (getTempAndUmid(vec, sensor)){
            printf("Umidade: %d.%d / Temperatura: %d.%d C \n", vec[0], vec[1], vec[2], vec[3]);

            // Converte os valores em string
            temp = integer2string(vec[2]);
            umid = integer2string(vec[0]);

            // ------------------------------------------- Escrita da Temperatura
            cmd_LCD(0x01,0); // Limpa o display
            cmd_LCD(0x80,0); // Coloca o cursor na primeira linha
            escreve_LCD_Flash(temperatura); // Escreve "Temp:"
            escreve_LCD(temp); // Escreve a temperatura lida pelo sensor
            
            // Escreve "ºC"
            escreve_LCD(" ");
            cmd_LCD(0xDF,1);
            escreve_LCD("C");
            
            // ------------------------------------------- Escrita da Umidade
            cmd_LCD(0xC0,0); // Coloca o cursor na segunda linha
            escreve_LCD_Flash(umidade); // Escreve "Umid:"
            escreve_LCD(umid); // Escreve a umidade lida pelo sensor
            escreve_LCD(" %");

            // Move a mensagem pra direita
            for(i=0; i<4; i++){
                cmd_LCD(0x1C, 0);
                _delay_ms(500);
            }

            // Move a mensagem pra esquerda
            for(i=0; i<4; i++){
                cmd_LCD(0x18, 0);
                _delay_ms(500);
            }

            // Espera e limpa as strings
            _delay_ms(500);
            free(temp);
            free(umid);
        }
    }
}
//================================================================================ //
