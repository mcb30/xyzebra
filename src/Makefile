LIBUSB_CFLAGS := $(shell pkg-config --cflags libusb-1.0)
LIBUSB_LDFLAGS := $(shell pkg-config --libs libusb-1.0)
CFLAGS += -W -Wall -Werror -O2

all : simple

simple : simple.o dlpusb245.o
	$(CC) -o $@ $^ $(LIBUSB_LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) $(LIBUSB_CFLAGS) -o $@ -c $<

clean :
	rm -f *.o
	rm -f simple
