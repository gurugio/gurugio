

LIB_SRCS = thrsafe.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
LIB_NAME = libthrsafe_test.a
LIB_DEBUG_OBJS = $(LIB_SRCS:.c=_d.o)
LIB_DEBUG_NAME = libthrsafe_test_d.a

CFLAGS = -O2 -W -Wall
LFLAGS = -L./ -lthrsafe_test -lpthread

all:$(LIB_NAME) $(LIB_DEBUG_NAME) example_test

example_test:example.c
	gcc $(CFLAGS) -o example_test $< $(LFLAGS)
	./example_test

$(LIB_NAME):$(LIB_SRCS)
	gcc -O2 -W -Wall -c -o $(LIB_OBJS) $(LIB_SRCS)
	ar rc $@ $(LIB_OBJS)

$(LIB_DEBUG_NAME):$(LIB_SRCS)
	gcc -O2 -W -Wall -c -o $(LIB_DEBUG_OBJS) $(LIB_SRCS)
	ar rc $@ $(LIB_DEBUG_OBJS)

clean:
	rm -f example_test *.o *.a

