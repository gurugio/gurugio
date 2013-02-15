
CFLAGS = -O2 -W -Wall

LIB_SRCS = atomic_list.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
LIB_NAME = libatomic_list.a
LIB_DEBUG_OBJS = $(LIB_SRCS:.c=_d.o)
LIB_DEBUG_NAME = libatomic_list_d.a


all:$(LIB_NAME) $(LIB_DEBUG_NAME) test

$(LIB_NAME):$(LIB_SRCS)
	gcc $(CFLAGS) -c $(LIB_SRCS)
	ar rc $@ $(LIB_OBJS)

$(LIB_DEBUG_NAME):$(LIB_SRCS)
	gcc $(CFLAGS) -g -DDEBUG=1 -c $(LIB_SRCS)
	ar rc $@ $(LIB_OBJS)

test:
	make -C test

clean:
	rm -f *.a *.o
