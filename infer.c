#include <stdbool.h>
#include "stlc.h"

#define FROM_ATOM_TO(A) (A)
#define TO_ATOM_FROM(A) (~(A))

static T infer1(T *, T *, T *, T, T);
static bool unify(T *, T, T);
static bool occurs(T *, T, T);

/* Infers the simple type of a given lambda term, or if the term has no simple
 * type then NIL is returned instead.
 *   term: A valid lambda term, such that its weight is not NIL.
 *   type: A array of T's, its length being at least the weight of the term.
 * The contents of `type' need not be initialised.
 */
T
infer(T *term, T *type)
{
	T fresh = 0;

	return infer1(term, type, &fresh, NIL, ROOT);
}

T
infer1(T *term, T *type, T *fresh, T parent, T t)
{
	if (ISABS(t)) {
		T xa = *fresh; *fresh += 2;
		T x = ATOM(xa);
		VALUE(x) = NIL;
		BINDTYPE(t) = TYPE(x);
		T a = infer1(term, type, fresh, t, BODY(t));
		if (a == NIL)
			return NIL;

		type[xa] = FROM_ATOM_TO(a);
		return xa;
	}
	else if (ISVAR(t)) {
		return TYPE(BINDTYPE(BINDER(t)));
	}
	else {
		T a = infer1(term, type, fresh, t, LEFT(t));
		if (a == NIL)
			return NIL;

		T b = infer1(term, type, fresh, t, RIGHT(t));
		if (b == NIL)
			return NIL;

		T bx = *fresh; *fresh += 2;
		T x = ATOM(bx);
		VALUE(x) = NIL;
		type[bx] = TO_ATOM_FROM(b);
		if (!unify(type, a, bx))
			return NIL;

		return x;
	}
}

bool
unify(T *type, T a, T b)
{
	while (ISATOM(a) && VALUE(a) != NIL)
		a = VALUE(a);
	while (ISATOM(b) && VALUE(b) != NIL)
		b = VALUE(b);

	if (a == b)
		return true;

	if (ISATOM(a)) {
		if (occurs(type, a, b))
			return false;
		VALUE(a) = b;
	}
	else if (ISATOM(b)) {
		if (occurs(type, b, a))
			return false;
		VALUE(b) = a;
	}
	else {
		if (!unify(type, DOM(a), DOM(b)) || !unify(type, COD(a), COD(b)))
			return false;
	}

	return true;
}

bool
occurs(T *type, T a, T b)
{
	while (a != b) {
		if (ISATOM(b)) {
			return false;
		}
		else {
			T c = ATOM(b);

			if (VALUE(c) != NIL) {
				do {
					c = VALUE(c);
				} while (ISATOM(c) && VALUE(c) != NIL);

				if (occurs(type, a, c))
					break;
			}
			b = SUB(b);
		}
	}

	return true;
}
