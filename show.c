#include <stdio.h>
#include <stdbool.h>
#include "stlc.h"

static void showterm1(const term_t *, const type_t *, term_t);
static void showtype1(const term_t *, const type_t *, type_t);

void
showterm(const term_t *term, const type_t *type)
{
	showterm1(term, type, ROOT);

	if (type) {
		putchar(' ');
		putchar(':');
		putchar(' ');
		showtype1(term, type, TYPEOF(ROOT));
	}

	putchar('\n');
}

void
showterm1(const term_t *term, const type_t *type, term_t t)
{
	bool bracket = true;

	if (ISVAR(t)) {
		printf("%ld", BINDER(t));
	}
	else if (ISABS(t)) {
		if (bracket) putchar('(');
		printf("\\%ld", t);
		if (type) {
			putchar(':');
			showtype1(term, type, ATOM(t));
		}
		putchar('.');
		putchar(' ');
		showterm1(term, type, BODY(t));
		if (bracket) putchar(')');
	}
	else {
		if (bracket) putchar('(');
		showterm1(term, type, LEFT(t));
		putchar(' ');
		showterm1(term, type, RIGHT(t));
		if (bracket) putchar(')');
	}
}

void
showtype1(const term_t *term, const type_t *type, type_t x)
{
	bool bracket = true;

	while (ISATOM(x) && HASVALUE(x))
		x = VALUE(x);

	if (ISATOM(x)) {
		printf("%ld", ~x);
	}
	else {
		if (bracket) putchar('(');
		showtype1(term, type, DOMAIN(x));
		putchar('-');
		putchar('>');
		showtype1(term, type, CODOMAIN(x));
		if (bracket) putchar(')');
	}
}
