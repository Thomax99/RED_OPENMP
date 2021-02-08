SRC= openmp_reductions_tests.c hello.c
EXE=$(SRC:.c=)
CFLAGS=-D_REENTRANT
LDLIBS=-fopenmp -lm

ALL: $(EXE)

clean:
	rm $(EXE)
