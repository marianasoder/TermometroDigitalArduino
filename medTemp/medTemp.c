#ifndef F_CPU
#define F_CPU 16000000UL //define a frequência do microcontrolador - 16MHz
#endif
#include "./medTemp.h"
#include <util/delay.h>

#define set_bit(y,bit) (y|=(1<<bit)) //coloca em 1 o bit x da variável Y
#define clr_bit(y,bit) (y&=~(1<<bit)) //coloca em 0 o bit x da variável Y
#define tst_bit(y,bit) (y&(1<<bit)) //retorna 0 ou 1 conforme leitura do bit

void startTempAndUmidSen(){
        _delay_ms(1000);
}

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

    return ((vec[0]+vec[1]+vec[2]+vec[3]) == vec[4]);
}