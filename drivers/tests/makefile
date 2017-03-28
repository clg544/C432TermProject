all: timer_test.o uart_test.o irq_test.o
%.o: %.s
	$(CC) $(CFLAGS) $(INCLUDE) -c $^
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $^;\
	mv ./$@ ../$@;
