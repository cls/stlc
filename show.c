#include <stdio.h>
#include "stlc.h"

void
showterm(const term_t *term, const type_t *type, term_t t)
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
showtype(const term_t *term, const type_t *type, type_t x)
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
