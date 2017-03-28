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
CROSS_COMPILE = /usr/local/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-
CC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
LD=$(CROSS_COMPILE)ld
AS=$(CROSS_COMPILE)as
Kernel=timer_test.bin
CFLAGS= -Wall -Wextra --pedantic -mcpu=cortex-a8 -mtune=cortex-a8 -march=armv7-a -mlong-calls -fdata-sections -funsigned-char -mfpu=neon -mfloat-abi=softfp 
LDFLAGS= -e Entry -u Entry -u __aeabi_uidiv -u __aeabi_idiv --gc-sections
Generic= start.o gpio.o clock.o timer.o uart.o interrupt_handler.o led.o
 
all: bootable.img cpheader.bin
	
gpheader.s: $(Kernel)
	FILESIZE=$$(stat -c%s "$(Kernel)") ;\
	echo ".data" | cat > gpheader.s ;\
	echo ".word $$FILESIZE" | cat >> gpheader.s ;\
	echo ".word 0x402F0400" | cat >> gpheader.s ;
bootable.img: gpheader.bin $(Kernel) 
	cat gpheader.bin $(Kernel) > bootable.img
	
led_test.elf: $(Generic) led_test.o 
	$(LD) $(LDFLAGS) $^ -T memmap.ld -o led_test.elf
timer_test.elf: $(Generic)  timer_test.o 
	$(LD) $(LDFLAGS) $^ -T memmap.ld -o timer_test.elf
uart_test.elf: $(Generic) uart_test.o 
	$(LD) $(LDFLAGS) $^ -T memmap.ld -o uart_test.elf
timer_test.o:
	$(CC) $(CFLAGS) -c 
%.o: $(patsubst %.o,,$@)/%.o
     cd $(patsubst %.o,,$@);\
     make;\
     mv ./$@ ../$@ ;\
%.bin: %.elf
	$(CROSS_COMPILE)objcopy -O binary $^ $@
%.bin: %.o
	$(CROSS_COMPILE)objcopy -O binary $^ $@
output: $(Kernel)
	$(CROSS_COMPILE)objdump -D $(Kernel:.bin=.elf)
clean:
	rm *.o *.bin *.elf


