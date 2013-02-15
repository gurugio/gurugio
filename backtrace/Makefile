
# MUST have no optimization
CFLAGS = -rdynamic -fno-omit-frame-pointer -W -Wall 

all:
	gcc $(CFLAGS) -c btrace.c

test:
	gcc -o test_btrace_glib unittest_btrace_glib.c btrace.o
	gcc -o test_btrace_builtin unittest_btrace_builtin.c btrace.o
	./test_btrace_glib
	./test_btrace_builtin

clean:
	rm -f *.a *.o
