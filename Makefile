CC := gcc
CFLAGS := -Wall -g
LFLAGS := -Llib
LINK := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer
IFLAGS := -Iinclude
OBJECTS := Board.o FEN.o main.o misc.o GUI.o 
HEADER := Board.h FEN.h Enum.h misc.h GUI.h 

main: $(OBJECTS) 
	$(CC) $(LFLAGS) $(CFLAGS) $^ $(LINK) -o main
%.o: %.c $(HEADER)
	$(CC) -c $(IFLAGS) $(CFLAGS) $< -o $@

.PHONY: clean run
clean:
	rm -f *.o *.exe
run:
	make
	./main.exe