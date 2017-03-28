CC=arm-linux-gnueabi-gcc
CFLAGS=-ansi -Wall -Werror -Wextra -march=armv6 -msoft-float -fPIC -mapcs-frame -marm -g
LD=arm-linux-gnueabi-ld
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

