#ifndef STLC_H
#define STLC_H

typedef long T;

#define ROOT 0
#define NIL  (-1)

/* Terms */

#define ISABS(T) (term[(T)] < 0)
#define ISVAR(T) (term[(T)] <= parent)

#define LEFT(T)  ((T) + 1)
#define RIGHT(T) (term[(T)])

#define BODY(T) ((T) + 1)
#define TYPE(T) (~(T))

#define BINDER(T)   (term[(T)])
#define BINDTYPE(T) (term[(T)])

/* Types */

#define ISATOM(A) ((A) % 2)
#define VALUE(A)  (type[(A)])

#define SUB(A)  (type[(A)] < 0 ? ~type[(A)] : type[(A)])
#define ATOM(A) ((A) + 1)

#define DOM(A) (type[(A)] < 0 ? ~type[(A)] : ATOM(A))
#define COD(A) (type[(A)] >= 0 ? type[(A)] : ATOM(A))

/* Functions */

T infer(T *, T *);
T weigh(T, T *);
void showterm(T *, T *);
void showtype(T *, T);

#endif
