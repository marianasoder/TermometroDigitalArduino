CC = avr-gcc
CCOBJ = avr-objcopy

# Nome do programa principal
PROG = main
EXTEN = c

DEP = -C ./uart.c  ./LCD.c 

# Porta de comunicação com o Arduino
PORT = /dev/ttyACM0

# Escolha uma otimização dentre as seguintes:
# -O0 -O1 -O2 -O3 -Os
OPTIMIZE = -Os

# Configuração para o Arduino UNO
# -------------------------------
# Altere apenas se for utilizar outro modelo de Arduino
# Consulte o arquivo /arduino/avr/boards.txt para outras configurações
MCU_TARGET = atmega328p
UPLOAD_SPEED = 115200
UPLOAD_PROTOCOL = arduino

# ==========================================
# Não é necessário alterar as regras abaixo.
# ==========================================
main:
	$(CC) $(OPTIMIZE) -mmcu=atmega328p -DF_CPU=16000000UL -c -o $(PROG).o $(PROG).$(EXTEN)
	$(CC) $(DEP) -mmcu=atmega328p $(PROG).o -o $(PROG).elf
	$(CCOBJ) -O ihex -R .eeprom $(PROG).elf $(PROG).hex

install:
	avrdude -V -C /etc/avrdude.conf -p atmega328p -c $(UPLOAD_PROTOCOL) -P $(PORT) -b $(UPLOAD_SPEED) -D -Uflash:w:"$(PROG).hex":i

clean:
	rm $(PROG).elf
	rm $(PROG).hex
	rm $(PROG).o
