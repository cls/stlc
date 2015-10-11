#include "stlc.h"

static bool unify(const term_t *, type_t *, type_t, type_t);
static bool occurs(const term_t *, type_t *, type_t, type_t);

/* Attempts to infer a simple typing for a term, and returns true if successful.
 *   term: A valid lambda term.
 *   type: An uninitialised array the same length as the term.
 *   t:    A subterm index.
 */
bool
infer(const term_t *term, type_t *type, term_t t)
{
	while (!ISVAR(t)) {
		INIT(ATOM(t));

		if (ISAPP(t)) {
			return infer(term, type, RIGHT(t))
			    && infer(term, type, LEFT(t))
			    && unify(term, type, TYPEOF(LEFT(t)), t);
		}
		else
			t = BODY(t);
	}

	return true;
}

bool
unify(const term_t *term, type_t *type, type_t x, type_t y)
{
	while (ISATOM(x) && HASVALUE(x))
		x = VALUE(x);
	while (ISATOM(y) && HASVALUE(y))
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

			if (HASVALUE(b)) {
				do {
					b = VALUE(b);
				} while (ISATOM(b) && HASVALUE(b));

				if (occurs(term, type, a, b))
					break;
			}

			x = SUB(x);
		}
	}

	return true;
}
