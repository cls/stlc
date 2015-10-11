#include "stlc.h"

static bool unify(const T *, T *, T, T);
static bool occurs(const T *, T *, T, T);

/* Attempts to infer a simple typing for a term, and returns true if successful.
 *   len:  The size of the term.
 *   term: A valid lambda term.
 *   type: An uninitialised array the same length as the term.
 */
bool
infer(long len, const T *term, T *type)
{
	for (T t = ROOT; t < len; t++)
		type[t] = ATOM(t); // initialise atoms

	for (T t = ROOT; t < len; t++)
		if (ISAPP(t) && !unify(term, type, TYPEOF(LEFT(t)), t))
			return false;

	return true;
}

bool
unify(const T *term, T *type, T x, T y)
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
occurs(const T *term, T *type, T a, T x)
{
	while (a != x) {
		if (ISATOM(x)) {
			return false;
		}
		else {
			T b = ATOM(x);

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
