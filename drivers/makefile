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

CFLAGS=-std=gnu99 -marm -Wall --pedantic -mfpu=neon -mhard-float -mcpu=cortex-a8 -O0 -nostdlib -nostartfiles -ffreestanding -fstack-usage -Wstack-usage=256
LDFLAGS=

all: led_test.bin

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $^

.SUFFIXES: .s .o
.s.o:
	$(AS) -mfpu=neon -o $@ $^

led_test.bin: led_test.o start.o gpio.o clock.o
	$(LD) start.o gpio.o clock.o led_test.o -T memmap.ld -o led_test.elf
	$(OBJCOPY) led_test.elf led_test.bin -O binary

timer_test.bin: timer_test.o start.o gpio.o clock.o timer.o
	$(LD) start.o timer.o gpio.o clock.o timer_test.o -T memmap.ld -o timer_test.elf
	$(OBJCOPY) timer_test.elf timer_test.bin -O binary

irq_test.bin: irq_test.o interrupt.o rtc.o start.o gpio.o clock.o timer.o
	$(LD) start.o interrupt.o rtc.o timer.o gpio.o clock.o irq_test.o -T memmap.ld -o irq_test.elf
	$(OBJCOPY) irq_test.elf irq_test.bin -O binary

uart_test.bin: uart_test.o start.o gpio.o clock.o uart.o
	$(LD) $(LDFLAGS) start.o uart.o gpio.o clock.o uart_test.o -T memmap.ld -o uart_test.elf
	$(OBJCOPY) uart_test.elf uart_test.bin -O binary



clean:
	rm *.o *.bin *.elf *.su


