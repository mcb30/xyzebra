NAME		= rs232
OBJECTS		= main.o leds.o serial.o cnc.o targets.o buttons.o
HZ		= 1000000
CC_MCU		= atmega168
CFLAGS		= -DBAUD=9600
AVRDUDE_MCU	= m168
AVRDUDE_PROG	= buspirate
AVRDUDE_PORT	= /dev/ttyUSB0

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = size
CFLAGS += -Os -g -W -Wall -Wextra -Werror
CFLAGS += -mmcu=$(CC_MCU) -DF_CPU=$(HZ)

all : $(NAME).bin

install : $(NAME).bin
	avrdude -c $(AVRDUDE_PROG) -P $(AVRDUDE_PORT) -p $(AVRDUDE_MCU) -U $<

$(NAME).elf : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	$(SIZE) $@

%.o : %.c $(wildcard *.h) Makefile
	$(CC) $(CFLAGS) -o $@ -c $<

%.bin : %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

%.lst : %.elf
	$(OBJDUMP) -h -S $< > $@

clean :
	rm -f *.o *.elf *.bin *.lst
