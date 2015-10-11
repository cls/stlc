#include "stlc.h"

/* Returns whether the given bounded array represents a valid lambda term.
 *   len:   The number of elements in the given array.
 *   term:  An array of length `len' to be checked for validity.
 *   scope: An uninitialised array of length `len'.
 */
bool
valid(long len, const T *term, long *scope)
{
	long max = len;

	for (T t = ROOT; t < len; t++) {
		if (ISVAR(t)) {
			if (t < BINDER(t) || t >= scope[BINDER(t)])
				return false; // illegal binder
			if (t+1 != max)
				return false; // dead code
			scope[t] = 0;
			max = scope[t+1];
		}
		else if (ISAPP(t)) {
			if (LEFT(t) >= len)
				return false; // out of bounds
			scope[t] = 0;
			scope[LEFT(t)] = max;
			max = LEFT(t);
		}
		else if (ISABS(t)) {
			scope[t] = max;
		}
		else
			return false;
	}

	return true;
}
