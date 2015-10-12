#ifndef STLC_H
#define STLC_H

#include <stdbool.h>

typedef long term_t;
typedef long type_t;

#define NIL  0
#define ROOT 1

/* Terms */

#define ISABS(T)  (term[(T)] == (T))
#define ISAPP(T)  (term[(T)] > (T))
#define ISVAR(T)  (term[(T)] < 0)

#define LEFT(T)   (term[(T)])
#define RIGHT(T)  ((T) + 1)

#define BODY(T)   ((T) + 1)
#define BINDER(T) (-term[(T)])

/* Types */

#define ISATOM(X)   ((X) < 0)
#define TYPEOF(T)   (ISAPP(T) ? ATOM(T) : term[(T)]) /* T or ATOM(BINDER(T)) */

#define ATOM(T)     (-(T))
#define SUB(T)      (TYPEOF((T) + 1)) /* RIGHT(T) or BODY(T) */

#define VALUE(A)    (type[-(A)])

#define DOMAIN(T)   (ISABS(T) ? ATOM(T) : SUB(T))
#define CODOMAIN(T) (ISABS(T) ? SUB(T) : ATOM(T))

/* Functions */

type_t *infer(const term_t *);
bool valid(long, const term_t *);
void showterm(const term_t *, const type_t *, term_t);
void showtype(const term_t *, const type_t *, type_t);

#endif
