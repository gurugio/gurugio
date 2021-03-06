#include "keyboard.h"

#include "inline_asm.h"
#include "console.h"
#include "process.h"

#include "char_device.h"
#include "string.h"


#define TABLE_NORMAL 0
#define TABLE_CAPSLOCK 1
#define TABLE_SHIFT 2
#define TABLE_CAPS_N_SHIFT 3
#define TABLE_CTRL 4

static unsigned char scan_to_ascii[5][128] = {
	{ /* ascii table */
	0x00, 0x1b,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=', 0x08, 0x09,
	 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']', 0x0d, 0x00,  'a',  's',
	 'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';', 0x27,  '`', 0x00, 0x5c,  'z',  'x',  'c',  'v',
	 'b',  'n',  'm',  ',',  '.',  '/', 0x00,  '*', 0x00,  ' ', 0x00, 0x03, 0x03, 0x03, 0x03, 0x08,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,  '-', 0x00, 0x00, 0x00,  '+', 0x00,
	0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x5c, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0d, 0x00,  '/', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  '/', 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{ /* ascii table with capslock */
	0x00, 0x1b,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=', 0x08, 0x09,
	 'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  'O',  'P',  '[',  ']', 0x0d, 0x00,  'A',  'S',
	 'D',  'F',  'G',  'H',  'J',  'K',  'L',  ';', 0x27,  '`', 0x00, 0x5c,  'Z',  'X',  'C',  'V',
	 'B',  'N',  'M',  ',',  '.',  '/', 0x00,  '*', 0x00,  ' ', 0x00, 0x03, 0x03, 0x03, 0x03, 0x08,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,  '-', 0x00, 0x00, 0x00,  '+', 0x00,
	0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x5c, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0d, 0x00,  '/', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  '/', 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{ /* ascii with shift */
	0x00, 0x1b,  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',  '_',  '+', 0x7e, 0x7e,
	 'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  'O',  'P',  '{',  '}', 0x7e, 0x00,  'A',  'S',
	 'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':', 0x27,  '~', 0x00,  '|',  'Z',  'X',  'C',  'V',
	 'B',  'N',  'M',  '<',  '>',  '?', 0x00,  '*', 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  '-', 0x00, 0x00, 0x00,  '+', 0x00,
	0x00, 0x00, 0x01, 0x7f, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0d, 0x00,  '/', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  '/', 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{ /* ascii with capslock & shift */
	0x00, 0x1b,  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',  '_',  '+', 0x7e, 0x7e,
	 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '{',  '}', 0x7e, 0x00,  'a',  's',
	 'd',  'f',  'g',  'h',  'j',  'k',  'l',  ':', 0x27,  '~', 0x00,  '|',  'z',  'x',  'c',  'v',
	 'b',  'n',  'm',  '<',  '>',  '?', 0x00,  '*', 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,	
 	0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  '-', 0x00, 0x00, 0x00,  '+', 0x00,
	0x00, 0x00, 0x01, 0x7f, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0d, 0x00,  '/', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  '/', 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{ /* ascii with control */
	0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x7f, 0x7f,
	0x11, 0x17, 0x05, 0x12, 0x14, 0x19, 0x15, 0x09, 0x0f, 0x10, 0x02, 0x02, 0x0a, 0x00, 0x01, 0x13,
	0x04, 0x06, 0x07, 0x08, 0x0a, 0x0b, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x18, 0x03, 0x16,
	0x02, 0x0e, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	}
};


key_tag_t key;
//int keybd_major;
static char_device_struct_t keyboard_device;
char keyboard_name[] = "keyboard";


int keyboard_open(const char *name)
{
	if (caos_strcmp(name, keyboard_device.name) != 0)
		return -1;

	register_irq(IRQ_KEYBOARD, keyboard_isr);
	enable_irq(IRQ_KEYBOARD);

	key.status = 0;
	key.code = 0;
	return keyboard_device.major;
}

int keyboard_close(const char *name)
{
	if (caos_strcmp(name, keyboard_device.name) != 0)
		return -1;

	unregister_irq(IRQ_KEYBOARD);
	disable_irq(IRQ_KEYBOARD);

	return 0;
}


int keyboard_read(char *buf, int size, int offset)
{
	int l;

	//if (fore_task->key_offset != 0)
	//	caos_printf("offset=%d\n", fore_task->key_offset);
	for (l=0; l<fore_task->key_offset; l++) {
		buf[l] = fore_task->key_input[l];
		//caos_printf("%c ", buf[l]);
		if (l > size)
			break;
	}
	
	fore_task->key_offset = 0;
	return l;

}

static file_operations_t keyboard_ops = {
	.read = keyboard_read,
	.write = NULL,
	.open = keyboard_open,
	.close = keyboard_close,
	.ioctl = NULL
};


int keyboard_init(void)
{

	caos_printf("Initialize keyboard..");

	keyboard_device.fops = &keyboard_ops;
	keyboard_device.name = keyboard_name;

	keyboard_device.major = register_chrdev(keyboard_name, &keyboard_device);

	if (keyboard_device.major < 0) {
		caos_printf("FAIL\n");
		return -1;
	}


	caos_printf("OK\n");
	return 0;
}

int device_exit(void)
{
	if (unregister_chrdev(keyboard_device.major, keyboard_device.name) < 0) {
		caos_printf("Release keyboard fail..\n");
		return -1;
	}

	return 0;
}


void keyboard_isr(void)
{
	unsigned char scan=0;
	static int prefix=0;
	int tmp;
	int change_flag = 0;

	scan = inb(0x60);
	//caos_printf("KEYBOARD [%x]\n", scan);

	// keyboard error
	if ( (scan == 0) || (scan == 0xff) )
		return;

	if (scan == 0xe0 || scan == 0xe1) {
		prefix = 1;
		return;
	}

	if (scan == SCAN_NUMLOCK) {
		//caos_printf("numlock pressed\n");
		tmp = MK_NUMLOCK;
		key.status ^= tmp;
		change_flag = 1;
	} else if (scan == SCAN_CAPSLOCK) {
		//caos_printf("capslock pressed\n");
		tmp = MK_CAPSLOCK;
		key.status ^= tmp;
		change_flag = 1;
	} else if (scan == SCAN_SCROLLLOCK) {
		//caos_printf("scroll-lock pressed\n");
		tmp = MK_SCROLLLOCK;
		key.status ^= tmp;
		change_flag = 1;
	} else if (scan == SCAN_LEFTSHIFT) {
		//caos_printf("left_shift pressed\n");
		tmp = MK_SHIFT;
		key.status |= tmp;
		change_flag = 1;
	} else if (scan == (SCAN_LEFTSHIFT|0x80)) {
		//caos_printf("left-shift released\n");
		tmp = MK_SHIFT;
		key.status &= ~tmp;
		change_flag = 1;
	/*} else if (scan == SCAN_RIGHTSHIFT) {
		//caos_printf("right-shift pressed\n");
		tmp = MK_SHIFT;
		key.status |= tmp;
		change_flag = 1;
	} else if (scan == (SCAN_RIGHTSHIFT|0x80)) {
		//caos_printf("right-shift released\n");
		tmp = MK_SHIFT;
		key.status &= ~tmp;
		change_flag = 1;*/
	} else if (scan == SCAN_LEFTCTRL) {
		//caos_printf("left-ctrl pressed\n");
		tmp = MK_CTRL;
		key.status |= tmp;
		change_flag = 1;
	} else if (scan == (SCAN_LEFTCTRL|0x80)) {
		//caos_printf("left-ctrl released\n");
		tmp = MK_CTRL;
		key.status &= ~tmp;
		change_flag = 1;
	} else if (scan == SCAN_LEFTALT) {
		//caos_printf("alt pressed\n");
		tmp = MK_ALT;
		key.status |= tmp;
		change_flag = 1;
	} else if (scan == (SCAN_LEFTALT|0x80)) {
		//caos_printf("alt released\n");
		tmp = MK_ALT;
		key.status &= ~tmp;
		change_flag = 1;
	}

	if (change_flag != 0) {
		// change keyboard LED
		//
		return;
	}

	//
	// Don't process key-releasing
	if (scan & 0x80)
		return;


	/*
	 * if no status, scan_to_ascii[0][scan]
	 * if capslock, scan_to_ascii[1][scan]
	 * ... ...
	 */
	if ( (key.status & MK_CAPSLOCK) && (key.status & MK_SHIFT) )
		key.code = scan_to_ascii[TABLE_CAPS_N_SHIFT][scan];
	else if (key.status == 0) 
		key.code = scan_to_ascii[TABLE_NORMAL][scan];
	else if (key.status & MK_CAPSLOCK) 
		key.code = scan_to_ascii[TABLE_CAPSLOCK][scan];
	else if (key.status & MK_SHIFT) 
		key.code = scan_to_ascii[TABLE_SHIFT][scan];
	else if (key.status & MK_CTRL) 
		key.code = scan_to_ascii[TABLE_CTRL][scan];
	else
		key.code = 0xff;	// 0xff is error code


	if (scan == SCAN_ENTER) {
		//caos_printf("\n");
		key_buffer_insert('\n');
		return;
	} else if (scan == SCAN_BACKSPACE) {
		key_buffer_insert('\b');
		//caos_delchar(1);
		return;
	}

	key_buffer_insert(key.code);



	prefix = 0;
}




void key_buffer_insert(char k)
{
	if (k != 0x0) {
		if (fore_task != NULL && fore_task->key_offset < KEY_INPUT_MAX) {
			fore_task->key_input[fore_task->key_offset] = k;
			fore_task->key_offset++;

			//caos_printf("%c", k);
		}
	}
}
















