#include "stlc.h"

/* Returns whether the given bounded array represents a valid lambda term.
 *   len:   The number of elements in the given array.
 *   term:  An array of length `len' to be checked for validity.
 *   scope: An uninitialised array of length `len'.
 *   t:     A subterm index.
 */
bool
valid(long len, const T *term, long *scope, T t)
{
	while (t < len) {
		if (ISVAR(t)) {
			scope[t] = 0;
			return BINDER(t) < t && t < scope[BINDER(t)];
		}
		else if (ISABS(t)) {
			scope[t] = len;
			t = BODY(t);
		}
		else {
			scope[t] = 0;
			if (!valid(LEFT(t), term, scope, RIGHT(t)))
				break;
			t = LEFT(t);
		}
	}

	return false; // fell out of bounds
}
