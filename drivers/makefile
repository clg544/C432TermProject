# This makefile is just for building programs for testing drivers.
#
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


