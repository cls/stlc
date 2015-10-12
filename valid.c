#include "stlc.h"

/* Returns whether the given bounded array represents a valid lambda term.
 *   len:   The number of elements in the given array.
 *   term:  An array of length `len' to be checked for validity.
 *   scope: An uninitialised array of length `len'.
 */
bool
valid(long len, const term_t *term, long *scope)
{
	long max = len;

	for (term_t t = ROOT; t < len; t++) {
		if (ISABS(t)) {
			scope[t] = max;
		}
		else if (ISAPP(t)) {
			if (LEFT(t) >= max)
				return false; // out of bounds
			scope[t] = 0;
			scope[LEFT(t)] = max;
			max = LEFT(t);
		}
		else {
			if (t >= scope[BINDER(t)])
				return false; // out of scope
			if (t+1 != max)
				return false; // dead code
			scope[t] = 0;
			max = scope[t+1];
		}
	}

	return true;
}
