%.o: %.s
	$(CC) $(CFLAGS) $(INCLUDE) -c $^
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $^;\
	mv ./$@ $(RELROOT)/objects/$@;
