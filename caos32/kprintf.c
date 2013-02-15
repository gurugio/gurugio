#include "types.h"
#include "console.h"
#include "string.h"
#include "io.h"


void __stack_chk_fail(void)
{
	return;
}


void scroll_screen(int line)
{
	int i;

	// This func can scroll several lines
	for (; line > 0; line--) {
		// move 2~25th lines to 1~24th lines
		// 2nd line -> 1st line
		// 3rd line -> 2nd line....
		for (i=0; i < SCREEN_ROW; i++)
			caos_memcpy(video_mem + i*ONELINE_SIZE /* previous line */\
					, video_mem + (i+1)*ONELINE_SIZE /* next line */\
					, ONELINE_SIZE /* line by line */ );
	}

}


int write_screen(const char *msg, int offset, int size)
{
	int l;
	char *vp = video_mem + offset*ONECHAR_SIZE;

	for (l=0; l<size; l++) {
		*(vp + ONECHAR_SIZE*l) = msg[l];
	}
	return l;
}

int clear_screen(int offset, int size)
{
	int l;
	char *vp = video_mem + offset*ONECHAR_SIZE;

	for (l=0; l<size; l++)
		*(vp + ONECHAR_SIZE*l) = 0;

	return l;
}


void init_screen(void)
{
		video_mem = (char *)VIDEO_ADDRESS;

		//
		// When C function starts, previous screen is deleted..
		cursor_offset = 0;
		set_cursor(cursor_offset);
}
		


void caos_putchar(char A_byteChar)
{
		*(video_mem + (cursor_offset*ONECHAR_SIZE) ) = A_byteChar;
		cursor_offset++;
		set_cursor(cursor_offset);
}


void caos_delchar(int cnt)
{
	
	for ( ; cnt > 0; cnt--) {
		cursor_offset--;
		set_cursor(cursor_offset);

		*(video_mem + (cursor_offset*ONECHAR_SIZE) ) = 0x0;
	}

}


void set_cursor(int A_dwNewCursorOffset)
{
		unsigned int dwTemp;

		/* set cursor to new position */
		cursor_offset = A_dwNewCursorOffset;


		// if cursor go over screen
		if ( (cursor_offset/SCREEN_COL) == SCREEN_ROW) {
			scroll_screen(1);
			cursor_offset -= SCREEN_COL;
		}

		/* port 0x3d4 :  */
		dwTemp = (unsigned int)(cursor_offset >> 8);
		outb((WORD)0x3d4, 0x0e);
		outb((WORD)0x3d5, dwTemp);

		dwTemp = (unsigned int)(cursor_offset & (unsigned int)0xff);
		outb((WORD)0x3d4, 0x0f);
		outb((WORD)0x3d5, dwTemp);

}

						

char sz[64];

int caos_printf(const char *format, ...)
{
		va_list ap;
		int i = 0;

		va_start(ap, format);
		caos_vsprintf(sz, format, ap);
		va_end(ap);

		while (sz[i] != '\0') {
			if (sz[i] == '\n') {
				// move to next line
				set_cursor( ((int)(cursor_offset/80) + 1)*80 );
				i++;
				continue;
			} else if (sz[i] == '\b') {
				caos_delchar(1);
				i++;
				continue;
			}

			caos_putchar(sz[i++]);
		}

		return i;
}


int print_hex(char *buf, int a, char flag)
{
	unsigned char *p;
	unsigned char v[10];
	int i=0, j=0;
	int count=0;

	if (a == 0) {
		*buf = 0x30;
		return 1;
	}

	p = (unsigned char *)&a;

	for (i = sizeof(int); i > 0; i--) {
		if ( (p[i-1] >> 4) <= 9)
			v[count] = (p[i-1] >> 4) + 0x30;
		else 
			v[count] = (p[i-1] >> 4) + 0x37;
		count++;

		if ( (p[i-1] & 0x0f) <= 9)
			v[count] = (p[i-1] & 0x0f) + 0x30;
		else 
			v[count] = (p[i-1] & 0x0f) + 0x37;
		count++;
	}


	i=0;
	while (v[i] == '0')	// skip initial zeros
		i++;
				
	count = 8 - i;	// how many digits
	for (j=0; j<count; j++)
		buf[j] = v[i++];

	return count;

}

void caos_vsprintf(char *A_szString, const char *A_szFormat, va_list A_pAp)
{
		int temp;
		

		while (*A_szFormat != '\0') {
			// print decimal number
			if (*A_szFormat == '%' && *(A_szFormat+1) == 'd') {
				temp = va_arg(A_pAp, int);
				temp = print_decimal(A_szString, temp, *(A_szFormat+1));
				A_szString += temp;
				A_szFormat += 2;
			// print hexa-decimal number
			} else if (*A_szFormat == '%' && *(A_szFormat+1) == 'x' ) {
				temp = va_arg(A_pAp, int);
				temp = print_hex(A_szString, temp, *(A_szFormat+1));
				A_szString += temp;
				A_szFormat += 2;
			// print scring
			} else if (*A_szFormat == '%' && *(A_szFormat+1) == 's') {
				temp = caos_strcat(A_szString, va_arg(A_pAp, char *));
				A_szString += temp;
				A_szFormat += 2;
			// print one character
			} else if (*A_szFormat == '%' && *(A_szFormat+1) == 'c') {
				*A_szString++ = va_arg(A_pAp, char);
				A_szFormat += 2;
			// normal character is copied as itself
			} else {
				*A_szString++ = *A_szFormat++;
			}
		}
		*A_szString = '\0';

}


int print_decimal(char *A_szBuffer, int a, char flag)
{
	unsigned char v[10];
	int i = 0;
	int count = 0;
		
	if (a == 0) {
		*A_szBuffer = 0x30;
		return 1;
	}

	/* print signed integer */
	if (a & 0x80000000) {
		/* print minus */
		*A_szBuffer = '-';
		count++;
		a = -a;
	}
				
	while (a > 0) {
		v[i++] = a % 10;
		a /= 10;
	}

	for (--i; i >= 0; i--) {
		*A_szBuffer++ = v[i]+0x30;
		count++;
	}
				

	return count;
}


