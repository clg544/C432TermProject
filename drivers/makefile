# This makefile is just for building programs for testing drivers.
#
# To upload to the beagle bone via serial, connect the serial adaper to the BBB
# install xmodem (linux package is usually called lrzsz) and picocom,
# and launch picocom with picocom -b 115200 /dev/ttyUSB0 --send-cmd "sx -vv".
# Power on the BBB with the boot mode switch (S2) depressed, picocom
# should start showing the character 'C' printed continually.
# Enter C-a C-s and enter the bin file name at the prompt.
# You may have to send it a few times for it to work
#
# TODO default to Ubuntu's tool chain name instead of Arch's
CROSS_COMPILE = arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
LD=$(CROSS_COMPILE)ld
AS=$(CROSS_COMPILE)as

CFLAGS=--specs=nosys.specs -Wall --pedantic

all: led_test.bin

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $^

start.o: start.s
	$(AS) start.s -o start.o

led_test.bin: led_test.o start.o gpio.o clock.o
	$(LD) start.o gpio.o clock.o led_test.o -T memmap.ld -o led_test.elf
	$(OBJCOPY) led_test.elf led_test.bin -O binary

clean:
	rm *.o *.bin *.elf


