#include <stdio.h>
#include "stlc.h"

#define LEN(X) (sizeof (X) / sizeof *(X))

//static T term[] = { 0, ~0 };
//static T term[] = { 0, 3, ~0, ~0 };
static T term[] = { 0, 1, 4, ~1, ~0 };

int
main(int argc, char **argv)
{
	T meta[LEN(term)];

	if (!valid(LEN(term), term, meta, ROOT)) {
		puts("term not valid");
		return 1;
	}

	showterm(term, NULL, ROOT);
	putchar('\n');

	if (!infer(term, meta, ROOT)) {
		puts("term not well-typed");
		return 1;
	}

	showterm(term, meta, ROOT);
	putchar('\n');

	showtype(term, meta, TYPEOF(ROOT));
	putchar('\n');

	return 0;
}
