#include "stlc.h"

static T weigh1(T, T *, T *, T, T);

/* Calculates the weight of a given lambda term, being the total number of
 * applications and abstractions in the term. If the object does not represent
 * a valid lambda term then NIL is returned instead.
 *   len:  The number of elements in the given array.
 *   term: An array of T's, its length being at least `len'.
 */
T
weigh(T len, T *term)
{
	T scope[len];

	return weigh1(len, term, scope, NIL, ROOT);
}

T
weigh1(T max, T *term, T *scope, T parent, T t)
{
	for (T weight = 0; t >= 0 && t < max; weight += 2) {
		if (ISABS(t)) {
			scope[t] = max;
			parent = t;
			t = BODY(t);
		}
		else if (ISVAR(t)) {
			if (t >= scope[BINDER(t)])
				break; // var is outside the scope of its binder
			return weight;
		}
		else {
			scope[t] = NIL;
			T w = weigh1(RIGHT(t), term, scope, t, LEFT(t));
			if (w == NIL)
				break;
			weight += w;
			parent = t;
			t = RIGHT(t);
		}
	}

	return NIL; // fell out of bounds
}
