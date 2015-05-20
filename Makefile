FLAGS = -std=c99 -Wall -Werror -O2 -march=native -g
LIBS = -lm
CC = gcc
EXE = diss_counter_example

$(EXE): diss_counter_example.c diss_counter.o
	$(CC) $(FLAGS) diss_counter_example.c diss_counter.o $(LIBS) -o $(EXE)
	
static: diss_counter.o
	ar rcs libdiss_counter.a diss_counter.o
	
diss_counter.o: diss_counter.c diss_counter.h
	$(CC) $(FLAGS) -fPIC -c diss_counter.c

clean:
	rm $(EXE)
