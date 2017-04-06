CROSS_COMPILE = /usr/local/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-
CC=$(CROSS_COMPILE)gcc
CFLAGS=-ansi -Wall -Werror -Wextra -march=armv6 -msoft-float -fPIC -mapcs-frame -marm -g
LD=$(CROSS_COMPILE)ld
LDFLAGS=-N -Ttext=0x10000

kernel.o:
	$(CC) $(CFLAGS) -c -o kernel.o kernel.c

.SUFFIXES: .o .elf
.o.elf:	
	$(LD) $(LDFLAGS) -o $@ $^

.SUFFIXES: .s .o
.s.o:
	$(CC) $(CFLAGS) -o $@ -c $^

kernel.elf: bootstrap.o context_switch.o syscalls.o kernel.o 
	$(LD) $(LDFLAGS) -o kernel.elf *.o

make clean:
	rm *.o 
	rm kernel.elf

