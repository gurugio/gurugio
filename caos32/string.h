#ifndef __STRING_H__

#define __STRING_H__

int caos_strcat(char *A_szDest, char *A_szSrc);
int caos_strcmp(const char *, char *);
int caos_strlen(const char *);
void *caos_memcpy(void *dst, void *src, unsigned int len);

#endif
