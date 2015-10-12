#include <stdlib.h>
#include "stlc.h"

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

bool
unify(const term_t *term, type_t *type, type_t x, type_t y)
{
	while (ISATOM(x) && VALUE(x) != NIL)
		x = VALUE(x);
	while (ISATOM(y) && VALUE(x) != NIL)
		y = VALUE(y);

	if (x == y) {
		return true;
	}
	else if (ISATOM(x)) {
		if (occurs(term, type, x, y))
			return false;
		VALUE(x) = y;
		return true;
	}
	else if (ISATOM(y)) {
		if (occurs(term, type, y, x))
			return false;
		VALUE(y) = x;
		return true;
	}
	else {
		return unify(term, type, DOMAIN(x),   DOMAIN(y))
		    && unify(term, type, CODOMAIN(x), CODOMAIN(y));
	}
}

bool
occurs(const term_t *term, type_t *type, type_t a, type_t x)
{
	while (a != x) {
		if (ISATOM(x)) {
			return false;
		}
		else {
			type_t b = ATOM(x);

			if (VALUE(b) != NIL) {
				do {
					b = VALUE(b);
				} while (ISATOM(b) && VALUE(b) != NIL);

				if (occurs(term, type, a, b))
					break;
			}

			x = SUB(x);
		}
	}

	return true;
}
