
CFLAGS = -O2 -W -Wall
LFLAGS = -L../atomic_list -latomic_list -I../atomic_list


TEST_SRC = main.c desc.c heap.c
TEST_EXE = alloc_test

all:$(TEST_EXE)

$(TEST_EXE):$(TEST_SRC)
	gcc -O2 -W -Wall -o $@ $(TEST_SRC) $(LFLAGS)
	gtags
	./$@

clean:
	rm -f $(TEST_EXE)
