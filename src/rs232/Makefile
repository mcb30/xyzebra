AVR_CC = avr-gcc
AVR_OBJCOPY = avr-objcopy
AVR_OBJDUMP = avr-objdump
AVR_CFLAGS = $(CFLAGS) -Os -mmcu=atmega168

all :

%.o : %.c
	$(AVR_CC) $(AVR_CFLAGS) -o $@ -c $<

%.bin : %.elf
	$(AVR_OBJCOPY) -j .text -j .data -O binary $< $@

%.lst : %.elf
	$(AVR_OBJDUMP) -h -S $< > $@

clean :
	rm -f *.o *.elf *.bin *.lst
