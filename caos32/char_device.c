
#include "types.h"
#include "console.h"

#include "char_device.h"



#define CDEV_MAJOR_MAX 8


char_device_struct_t *cdev_table[CDEV_MAJOR_MAX];


void init_char_dev(void)
{
	int i;

	caos_printf("INIT CHAR DEVICE TABLE..\n");
	for (i=0; i<CDEV_MAJOR_MAX; i++) {
		cdev_table[i] = NULL;
	}

}



int register_chrdev(char *name, char_device_struct_t *cdev)
{
	int i;

	caos_printf("Register [%s] device..", name);

	for (i=0; i<CDEV_MAJOR_MAX; i++) {
		if (cdev_table[i] == NULL) {
			cdev_table[i] = cdev;
			caos_printf("OK\n", i);
			return i;
		}
	}
	
	caos_printf("FAIL\n");
	return -1;
}

int unregister_chrdev(int major_num, char *name)
{
	caos_printf("Unregister device number [%d]..");
	if (major_num >= CDEV_MAJOR_MAX || major_num < 0) {
		caos_printf("FAIL\n");
		return -1;
	}

	cdev_table[major_num] = NULL;
	caos_printf("OK\n");
	return 1;
}

