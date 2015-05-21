FLAGS = -std=c99 -Wall -Werror -O2 -march=native -g
LIBS = -lm
EXE = diss_counter

.PHONY: clean

$(EXE): diss_counter_example.c libdiss_counter.a
	cc -static $(FLAGS) diss_counter_example.c -L. -ldiss_counter $(LIBS) -o $(EXE)

static: libdiss_counter.a

libdiss_counter.a: diss_counter.o
	ar rcs libdiss_counter.a diss_counter.o

dynamic: diss_counter.o
	gcc -shared -Wl,-soname,libdiss_counter.so.0 -o libdiss_counter.so.0.0.1 diss_counter.o

diss_counter.o: diss_counter.c diss_counter.h
	$(CC) $(FLAGS) -fPIC -c diss_counter.c

clean:
	rm $(EXE) libdiss_counter.a
