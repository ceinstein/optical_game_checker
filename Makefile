CC=gcc
OUTPUT=sudoku
INC=./include
all:
	$(CC) src/*.c -o bin/$(OUTPUT) -I$(INC)

make debug:
	$(CC) src/*.c -o bin/$(OUTPUT) -I$(INC) -DDEBUG

run:
	./bin/$(OUTPUT)

clean:
	rm -rf obj/*
	rm -rf *~
	rm -rf src/*~
	rm -rf include/*~
	rm -rf bin/$(OUTPUT)
