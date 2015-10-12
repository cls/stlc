#include <stdlib.h>
#include "stlc.h"

static type_t find(type_t *, type_t);
static bool unify(const term_t *, type_t *, type_t, type_t);
static bool occurs(const term_t *, type_t *, type_t, type_t);

/* Attempts to infer a simple typing for a term. */
type_t *
infer(const term_t *term)
{
	long len = term[0];
	type_t *type;
	term_t t;

	if (!(type = calloc(len, sizeof *type)))
		return NULL;

	for (t = ROOT; t < len; t++) {
		if (ISAPP(t) && !unify(term, type, TYPEOF(LEFT(t)), t)) {
			free(type);
			return NULL;
		}
	}

	return type;
}

type_t
find(type_t *type, type_t x)
{
	if (ISATOM(x) && VALUE(x) != NIL)
		return VALUE(x) = find(type, VALUE(x)); /* path compression */
	else
		return x;
}

bool
unify(const term_t *term, type_t *type, type_t x, type_t y)
{
	for (;;) {
		x = find(type, x);
		y = find(type, y);

		if (x == y) {
			break;
		}
		else if (ISATOM(x)) {
			if (occurs(term, type, x, y))
				return false;
			VALUE(x) = y;
			break;
		}
		else if (ISATOM(y)) {
			if (occurs(term, type, y, x))
				return false;
			VALUE(y) = x;
			break;
		}
		else {
			if (!unify(term, type, DOMAIN(x), DOMAIN(y)))
				return false;
			x = CODOMAIN(x);
			y = CODOMAIN(y);
		}
	}

	return true;
}

bool
occurs(const term_t *term, type_t *type, type_t a, type_t x)
{
	for (;;) {
		if (x == a)
			return true;
		else if (ISATOM(x))
			return false;
		else if (occurs(term, type, a, find(type, ATOM(x))))
			return true;
		else
			x = SUB(x);
	}
}
