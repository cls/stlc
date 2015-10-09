#include <stdio.h>
#include <stdbool.h>
#include "stlc.h"

static void showterm1(T *, T *, T, T);
static void showtype1(T *, bool, T);

void
showterm(T *term, T *type)
{
	showterm1(term, type, NIL, ROOT);
	putchar('\n');
}

void
showtype(T *type, T a)
{
	showtype1(type, false, a);
	putchar('\n');
}

void
showterm1(T *term, T *type, T parent, T t)
{
	if (ISABS(t)) {
		bool bracket = parent != NIL && !ISABS(parent);

		if (bracket) putchar('(');
		printf("\\%ld", t);
		if (type) {
			putchar(':');
			showtype1(type, false, TYPE(BIND(t)));
		}
		putchar('.');
		putchar(' ');
		showterm1(term, type, t, BODY(t));
		if (bracket) putchar(')');
	}
	else if (ISVAR(t)) {
		printf("%ld", BINDER(t));
	}
	else {
		bool bracket = parent != NIL && !ISABS(parent) && t == RIGHT(parent);

		if (bracket) putchar('(');
		showterm1(term, type, t, LEFT(t));
		putchar(' ');
		showterm1(term, type, t, RIGHT(t));
		if (bracket) putchar(')');
	}
}

void
showtype1(T *type, bool bracket, T a)
{
	while (ISATOM(a) && VALUE(a) != NIL)
		a = VALUE(a);

	if (ISATOM(a)) {
		printf("%ld", a / 2);
	}
	else {
		if (bracket) putchar('(');
		showtype1(type, true, DOM(a));
		putchar('-');
		putchar('>');
		showtype1(type, false, COD(a));
		if (bracket) putchar(')');
	}
}
