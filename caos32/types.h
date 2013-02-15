#ifndef __TYPES_H__

#define __TYPES_H__

#define NULL (void *)0

typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

typedef unsigned long ULONG;
typedef unsigned char UCHAR;
typedef char *va_list;

#define va_start(ap, last) (void)((ap)=(va_list)&(last)+sizeof(last))


#define  _AUPBND                (sizeof (int) - 1)
#define  _ADNBND                (sizeof (int) - 1)
#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))


//#define va_arg(ap, type) (*((type *)ap)++)
//#define va_arg(argp, type) (*(type *)(((argp) += sizeof(type))))
//#define	va_arg(ap,type)		*(type *)ap; ap += sizeof(int)

#define va_end(ap) (void)((ap)=NULL)

#endif
