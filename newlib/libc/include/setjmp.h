/*
	setjmp.h
	stubs for future use.
*/

#ifndef _SETJMP_H_
#define _SETJMP_H_

#include "_ansi.h"
#include <machine/setjmp.h>

_BEGIN_STD_C

#ifdef __GNUC__
void	longjmp (jmp_buf __jmpb, int __retval)
			__attribute__ ((__noreturn__));
#else
void	longjmp (jmp_buf __jmpb, int __retval);
#endif

#ifdef __GNUC__
int	setjmp (jmp_buf __jmpb)
			__attribute__((returns_twice));
#else
int	setjmp (jmp_buf __jmpb);
#endif

_END_STD_C

#endif /* _SETJMP_H_ */
