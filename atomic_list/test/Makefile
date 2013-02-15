
CFLAGS = -O2 -W -Wall
LFLAGS = -L../../threadsafety_test -lthrsafe_test -lpthread

UNIT_SRC = test_atomic_list_unittest.c
UNIT_EXE = $(UNIT_SRC:.c=)

INSERT_SRC = test_atomic_list_insert.c
INSERT_EXE = $(INSERT_SRC:.c=)

DELETE_SRC = test_atomic_list_delete.c
DELETE_EXE = $(DELETE_SRC:.c=)

DELETE_FIRST_SRC = test_atomic_list_delete_first.c
DELETE_FIRST_EXE = $(DELETE_FIRST_SRC:.c=)

ALL_SRC = test_atomic_list_all.c
ALL_EXE = $(ALL_SRC:.c=)

all:build

build:../atomic_list.c
	make -C ../../threadsafety_test
	gcc $(CFLAGS) -o $(UNIT_EXE) $(UNIT_SRC) ../atomic_list.c
	gcc $(CFLAGS) -o $(INSERT_EXE) $(INSERT_SRC) ../atomic_list.c $(LFLAGS)
	gcc $(CFLAGS) -o $(DELETE_EXE) $(DELETE_SRC) ../atomic_list.c $(LFLAGS)
	gcc $(CFLAGS) -o $(DELETE_FIRST_EXE) $(DELETE_FIRST_SRC) ../atomic_list.c $(LFLAGS) 
	gcc $(CFLAGS) -o $(ALL_EXE) $(ALL_SRC) ../atomic_list.c $(LFLAGS)
	@echo "==> run \"make test\" to execute tests"

test:
	./$(UNIT_EXE)
	./$(INSERT_EXE)
	./$(DELETE_EXE)
	./$(DELETE_FIRST_EXE)
	./$(ALL_EXE)
	valgrind --tool=memcheck --leak-resolution=high --leak-check=yes ./$(DELETE_FIRST_EXE)
	valgrind --tool=memcheck --leak-resolution=high --leak-check=yes ./$(ALL_EXE)

clean:
	rm -f $(UNIT_EXE) $(INSERT_EXE) $(DELETE_EXE) $(DELETE_FIRST_EXE) $(ALL_EXE)
