#include <stdio.h>
#include "stlc.h"

void
showterm(const T *term, const T *type, T t)
{
	if (ISVAR(t)) {
		printf("%ld", BINDER(t));
	}
	else if (ISABS(t)) {
		putchar('(');
		printf("\\%ld", t);
		if (type) {
			putchar(':');
			showtype(term, type, ATOM(t));
		}
		putchar('.');
		putchar(' ');
		showterm(term, type, BODY(t));
		putchar(')');
	}
	else {
		putchar('(');
		showterm(term, type, LEFT(t));
		putchar(' ');
		showterm(term, type, RIGHT(t));
		putchar(')');
	}
}

void
showtype(const T *term, const T *type, T x)
{
	while (ISATOM(x) && HASVALUE(x))
		x = VALUE(x);

	if (ISATOM(x)) {
		printf("%ld", ATOM(x));
	}
	else {
		putchar('(');
		showtype(term, type, DOMAIN(x));
		putchar('-');
		putchar('>');
		showtype(term, type, CODOMAIN(x));
		putchar(')');
	}
}
