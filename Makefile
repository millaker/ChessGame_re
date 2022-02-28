CC := gcc
CFLAGS := -Wall -g
LFLAGS :=
OBJECTS := Board.o FEN.o main.o misc.o
HEADER := Board.h FEN.h Enum.h misc.h

main: $(OBJECTS) 
	$(CC) $(CFLAGS) $^ -o main
%.o: %.c $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean run
clean:
	rm -f *.o *.exe
run:
	make
	./main.exe