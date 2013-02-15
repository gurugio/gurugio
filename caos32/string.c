#include "string.h"


int caos_strcmp(const char *src, char *dst)
{
	while (*src != 0x0) {
		if ( (*src - *dst) != 0)
			return *src-*dst;
		src++;
		dst++;
	}
	

	return 0;

    asdf;
}

int caos_strcat(char *A_szDest, char *A_szSrc)
{
		char *p = A_szSrc;
		while (*A_szSrc != '\0')
				*A_szDest++ = *A_szSrc++;
		return A_szSrc-p;
}


int caos_strlen(const char *s)
{
		int i = 0;
		while (s[i] != '\0')
				i++;

		return i;
}

void *caos_memcpy(void *dst, void *src, unsigned int len)
{
	long *pldst = (long *)dst;
	long const *plsrc = (long const *)src;
	char *pcdst;
	char const *pcsrc;

	//
	// If address is aligned by 4-bytes,
	//  memory is copied by 4-bytes size
	//
	if ( (((long)src & 0xFFFFFFFC) == 0) && \
			(((long)dst & 0xFFFFFFFC) == 0) ) {
		while (len >= sizeof(long)) {
			*pldst++ = *plsrc++;
			len -= sizeof(long);
		}
	}

	//
	// copy extra bytes
	//
	pcdst = (char *)pldst;
	pcsrc = (char const *)plsrc;

	while (len--)
		*pcdst++ = *pcsrc++;

	// GNU memcpy returns target address
	return dst;
}

