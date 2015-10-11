#ifndef STLC_H
#define STLC_H

#include <stdbool.h>

typedef long T;

#define ROOT 0

/* Terms */

#define ISVAR(T) (term[(T)] < 0)
#define ISABS(T) (term[(T)] == (T))
#define ISAPP(T) (term[(T)] > (T))

#define LEFT(T)  (term[(T)])
#define RIGHT(T) ((T) + 1)
#define BODY(T)  ((T) + 1)

#define VARTYPE(T) (term[(T)])
#define BINDER(T)  (ATOM(VARTYPE(T)))

/* Types */

#define ISATOM(X)   ((X) <  0)
#define ISFUNC(X)   ((X) >= 0)

#define TYPEOF(T)   (ISAPP(T) ? ATOM(T) : term[(T)]) // abs: T, var: VARTYPE(T)

#define ATOM(T)     (~(T))
#define SUB(T)      (TYPEOF((T) + 1)) // RIGHT or BODY

#define VALUE(A)    (type[ATOM(A)])
#define HASVALUE(A) (VALUE(A) != (A))
#define INIT(A)     (VALUE(A) = A)

#define DOMAIN(T)   (ISABS(T) ? ATOM(T) : SUB(T))
#define CODOMAIN(T) (ISABS(T) ? SUB(T) : ATOM(T))

/* Functions */

bool infer(const T *, T *, T);
bool valid(long, const T *, long *, T);
void showterm(const T *, const T *, T);
void showtype(const T *, const T *, T);

#endif
