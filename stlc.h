#ifndef STLC_H
#define STLC_H

#include <stdbool.h>

typedef unsigned long term_t;
typedef long type_t;

#define ROOT 0

/* Terms */

#define ISABS(T)  (term[(T)] == (T))
#define ISAPP(T)  (term[(T)] > (T))
#define ISVAR(T)  (term[(T)] < (T))

#define LEFT(T)   (term[(T)])
#define RIGHT(T)  ((T) + 1)

#define BODY(T)   ((T) + 1)
#define BINDER(T) (term[(T)])

/* Types */

#define ISATOM(X)   ((X) < 0)
#define TYPEOF(T)   (ISABS(T) ? (T) : ATOM(T))

#define ATOM(T)     (~(T))
#define SUB(T)      (TYPEOF((T) + 1)) // RIGHT or BODY

#define VALUE(A)    (type[~(A)])
#define HASVALUE(A) (VALUE(A) != (A))

#define DOMAIN(T)   (ISABS(T) ? ATOM(T) : SUB(T))
#define CODOMAIN(T) (ISABS(T) ? SUB(T) : ATOM(T))

/* Functions */

bool infer(long, const term_t *, type_t *);
bool valid(long, const term_t *, long *);
void showterm(const term_t *, const type_t *, term_t);
void showtype(const term_t *, const type_t *, type_t);

#endif
