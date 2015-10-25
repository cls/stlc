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

	type_t *type = calloc(len, sizeof *type);
	if (!type)
		return NULL;

	for (type_t t = ROOT; t < len; t++) {
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
		return VALUE(x) = find(type, VALUE(x)); // path compression
	else
		return x;
}

bool
unify(const term_t *term, type_t *type, type_t x, type_t y)
{
	for (; (x = find(type, x)) != (y = find(type, y)); x = COD(x), y = COD(y))
		if (ISATOM(x))
			return !occurs(term, type, x, y) && (VALUE(x) = y, true);
		else if (ISATOM(y))
			return !occurs(term, type, y, x) && (VALUE(y) = x, true);
		else if (!unify(term, type, DOM(x), DOM(y)))
			return false;

	return true;
}

bool
occurs(const term_t *term, type_t *type, type_t a, type_t x)
{
	for (; !ISATOM(x); x = SUB(x))
		if (occurs(term, type, a, find(type, ATOM(x))))
			return true;

	return x == a;
}
