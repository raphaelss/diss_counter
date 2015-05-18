FLAGS = -std=c99 -O2 -march=native -g
LIBS = -lm
CC = gcc
EXE = diss_counter_example

$(EXE): diss_counter_example.c diss_counter.c diss_counter.h
	$(CC) $(FLAGS) diss_counter_example.c diss_counter.c $(LIBS) -o $(EXE)

clean:
	rm $(EXE)
