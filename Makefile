FLAGS = -std=c11 -O3 -march=native -flto
LIBS = -lm
CC = gcc
EXE = diss_counter_example

$(EXE): diss_counter_example.c diss_counter.c diss_counter.h
	$(CC) $(FLAGS) diss_counter_example.c diss_counter.c $(LIBS) -o $(EXE)

clean:
	rm $(EXE)
