#ifndef __CONSOLE_H__

#define __CONSOLE_H__

#include "types.h"

#define VIDEO_ADDRESS 0xc00b8000
#define SCREEN_ROW 25
#define SCREEN_COL 80
#define ONECHAR_SIZE 2
#define ONELINE_SIZE (SCREEN_COL * ONECHAR_SIZE)


int caos_printf(const char *format, ...);
int print_hexa(char *, int a, char flag);
int print_decimal(char *, int a, char flag);
void caos_vsprintf(char *A_szString, const char *A_szFormat, va_list A_pAp);


void init_screen(void);
void caos_putchar(char);
void caos_delchar(int);
void set_cursor(int);

void scroll_screen(int);
int write_screen(const char *, int, int);
int clear_screen(int, int);


//DWORD dwordCursorOffset;
//UCHAR *VRAM;


unsigned int cursor_offset;
char *video_mem;



#endif
