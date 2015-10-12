#include <stdlib.h>
#include "stlc.h"

/* Returns whether the given bounded array represents a valid lambda term. */
bool
valid(long len, const term_t *term)
{
	long max;
	long *scope;
	term_t t;

	if (len < 2)
		return false; /* no term present */
	if (term[0] != len)
		return false; /* wrong length */

	if (!(scope = malloc(len * sizeof *scope)))
		return false;

	max = len;

	for (t = ROOT; t != max; t++) {
		if (ISABS(t)) {
			scope[t] = max;
		}
		else if (ISAPP(t)) {
			if (LEFT(t) >= max)
				break; /* out of bounds */
			scope[t] = 0;
			scope[LEFT(t)] = max;
			max = LEFT(t);
		}
		else if (ISVAR(t)) {
			if (t >= scope[BINDER(t)])
				break; /* out of scope */
			if (t+1 != max)
				break; /* dead code */

			if (max == len) {
				free(scope);
				return true; /* end of array */
			}
			scope[t] = 0;
			max = scope[max];
		}
		else
			break;
	}

	free(scope);
	return false;
}
