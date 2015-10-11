#include "stlc.h"

static bool valid1(long, const term_t *, long *, term_t);

/* Returns whether the given bounded array represents a valid lambda term.
 *   len:   The number of elements in the given array.
 *   term:  An array of length `len' to be checked for validity.
 *   scope: An uninitialised array of length `len'.
 */
bool
valid(long len, const term_t *term, long *scope)
{
	return valid1(len, term, scope, ROOT);
}

bool
valid1(long max, const term_t *term, long *scope, term_t t)
{
	while(t < max) {
		if (ISVAR(t)) {
			scope[t] = 0;
			return t < scope[BINDER(t)];
		}
		else if (ISABS(t)) {
			scope[t] = max;
			t = BODY(t);
		}
		else {
			scope[t] = 0;
			if (!valid1(LEFT(t), term, scope, RIGHT(t)))
				break;
			t = LEFT(t);
		}
	}

	return false; // fell out of bounds
}
