
#include "user_syscall.h"


_syscall0(int, sys_syscall_test);
_syscall1(int, sys_cputstr, const char *, str);
_syscall2(int, sys_cgetstr, const char *, str, long, arg);

//
// sys_cprintf can treat only one value.
//
_syscall2(int, sys_cprintf, const char *, fmt, long, arg);

_syscall0(int, sys_system_info);
_syscall0(int, sys_turnoff);

_syscall2(int, sys_open, const char *, name, int, flag);
_syscall1(int, sys_close, int, fd);
_syscall3(int, sys_read, int, fd, char *, buf, int, count);
_syscall3(int, sys_write, int, fd, char *, buf, int, count);




