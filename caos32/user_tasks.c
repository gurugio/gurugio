

#include "user_tasks.h"
#include "user_syscall.h"

#define USER1_FLOW 11


void user1(void)
{
	char *bp;
	char flow[4] = {'-', '\\', '|', '/'};
	unsigned int time=0;



	while (1) {
		bp = (char *)0xb8000;
		*bp++ = '[';
		bp++;
		*bp++ = 'U';
		bp++;
		*bp++ = 'S';
		bp++;
		*bp++ = 'E';
		bp++;
		*bp++ = 'R';
		bp++;
		*bp++ = ' ';
		bp++;
		*bp++ = '#';
		bp++;
		*bp++ = '1';
		bp++;
		*bp++ = ']';
		bp++;
		time++;
		(*bp) = flow[time%4];
		bp++;
		*bp = ' ';
	}

}

void user2(void)
{
	char *bp;
	char flow[4] = {'-', '\\', '|', '/'};
	unsigned int time=0;


	while (1) {
		bp = (char *)0xb8000 + 2*USER1_FLOW;
		*bp++ = '[';
		bp++;
		*bp++ = 'U';
		bp++;
		*bp++ = 'S';
		bp++;
		*bp++ = 'E';
		bp++;
		*bp++ = 'R';
		bp++;
		*bp++ = ' ';
		bp++;
		*bp++ = '#';
		bp++;
		*bp++ = '2';
		bp++;
		*bp++ = ']';
		bp++;
		time++;
		(*bp) = flow[time%4];
		bp++;
		*bp = ' ';
	}
}
