LIBUSB_CFLAGS := $(shell pkg-config --cflags libusb-1.0)
LIBUSB_LDFLAGS := $(shell pkg-config --libs libusb-1.0)
CFLAGS += -W -Wall -Werror -O2

AVR_CC = avr-gcc
AVR_OBJCOPY = avr-objcopy
AVR_OBJDUMP = avr-objdump
AVR_CFLAGS = $(CFLAGS) -Os -mmcu=atmega168

all : simple tepctrl.bin

simple : simple.o dlpusb245.o
	$(CC) -o $@ $^ $(LIBUSB_LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) $(LIBUSB_CFLAGS) -o $@ -c $<

%.elf : %.c
	$(AVR_CC) $(AVR_CFLAGS) -o $@ $<

%.bin : %.elf
	$(AVR_OBJCOPY) -j .text -j .data -O binary $< $@

%.lst : %.elf
	$(AVR_OBJDUMP) -h -S $< > $@

clean :
	rm -f *.o *.elf *.bin *.lst
	rm -f simple
