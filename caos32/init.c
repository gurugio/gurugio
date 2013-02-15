
#include "init.h"
#include "user_syscall.h"



void init(void)
{

	int i, l;
	char buf[32];
	char temp[32];
	int ret;
	int keybd_dev;

	if ( (keybd_dev = sys_open("keyboard", 0)) < 0) {
		sys_cputstr("open keyboard fail\n");
		while(1);
	}

	sys_cputstr("SHELL START..\n");
	sys_cputstr("Type help for command list\n");
	

	while(1) {
		sys_cputstr("CaOS # ");

		while (1) {
			//if ( (ret = sys_cgetstr(temp, 32)) > 0) {
			if ( (ret = sys_read(keybd_dev, temp, 31)) > 0) {
				for (l=0; l<ret; l++) {
					buf[i] = temp[l];
					if (buf[i] == '\n') {
						sys_cprintf("%c", buf[i]);
						goto next_line;
					} else if (buf[i] == '\b') {
						if (i >= 1) {
							buf[i] = 0;
							buf[i-1] = 0;
							i--;
							sys_cputstr("\b");
						}
						continue;
					}

					sys_cprintf("%c", buf[i]);
					i++;
				}
			}
		}

next_line:
		buf[i] = 0;
		i=0;
		
		if (buf[0] == 'h') {
			sys_cputstr("s : system info\n");
			sys_cputstr("i : CaOS info\n");
			sys_cputstr("x : exit\n");
		} else if (buf[0] == 's') {
			sys_system_info();
		} else if (buf[0] == 'i') {
			sys_cputstr("CaOS v0.05 IA-32 i386\n");
		} else if (buf[0] == 'x') {
			sys_turnoff();
			sys_cputstr("TURN OFF PC..\n");
			while (1);
		} else {
			sys_cputstr("Unknown command\n");
		}
	}
	return;
}



