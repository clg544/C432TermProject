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
.PHONY: tests
CROSS_COMPILE = /usr/local/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-
#CROSS_COMPILE = arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy
LD=$(CROSS_COMPILE)ld 
AS=$(CROSS_COMPILE)as


#CFLAGS=-std=gnu99 -marm -Wall --pedantic -mfpu=neon -mhard-float -mcpu=cortex-a8 -O0 -nostdlib -nostartfiles -ffreestanding -fstack-usage -Wstack-usage=256
Kernel=debug_test.elf

CFLAGS= -Wall -Wextra --pedantic -mcpu=cortex-a8 -mtune=cortex-a8 -march=armv7-a -mlong-calls -fdata-sections -funsigned-char -mfpu=neon -mfloat-abi=softfp 
LDFLAGS= -e _start -u Entry -u __aeabi_uidiv -u __aeabi_idiv --gc-sections
Generic= start gpio clock timer uart interrupt_handler led cm debug aeabi
PWD=$(shell pwd)
RELROOT=$(PWD)
INCLUDE= -I$(PWD)/include/
OBJECTS=$(PWD)/objects
export CFLAGS
export LDFLAGS
export CC
export RELROOT
export INCLUDE
all: bootable.img cpheader.bin
	
gpheader.s: $(Kernel:.elf=.bin)
	FILESIZE=$$(stat -c%s "$(Kernel:.elf=.bin)") ;\
	echo ".data" | cat > gpheader.s ;\
	echo ".word $$FILESIZE" | cat >> gpheader.s ;\
	echo ".word 0x402F0400" | cat >> gpheader.s ;
bootable.img: gpheader.bin $(Kernel:.elf=.bin)
	cat gpheader.bin $(Kernel:.elf=.bin) > bootable.img

rtc.o: rtc.s
	$(CC) $(CFLAGS) $^ -c
	
%.elf: $(Generic:%=%.a) 
	$(MAKE) -C ./tests $(patsubst %.elf,%.o,$@) ; \
	cd ./objects ;\
	objlist=$$(ls *.o);\
	$(LD) $(LDFLAGS) $$objlist -T $(RELROOT)/memmap.ld -o $@;\
	mv ./$@ ../$@;
%.o: %.s
	$(CC) $(CFLAGS) $(INCLUDE) -c $^
%.a: 
	$(MAKE) -f$(PWD)/generic_makefile -C $(PWD)/$(patsubst %.a,%,$@);
gpheader.bin: gpheader.o
	$(OBJCOPY) -O binary $^ $@
cpheader.bin: cpheader.o
	$(OBJCOPY) -O binary $^ $@
%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@
output: $(Kernel)
	$(CROSS_COMPILE)objdump -D $(Kernel:.bin=.elf)
clean:
	cd objects ;\
	rm *.o;
	rm *.bin *.elf *.su *.img gpheader.s


