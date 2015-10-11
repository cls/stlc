#include <stdio.h>
#include "stlc.h"

#define LEN(X) (sizeof (X) / sizeof *(X))

//static term_t term[] = { 0, 0 };
//static term_t term[] = { 0, 3, 0, 0 };
static term_t term[] = { 0, 1, 4, 1, 0 };

int
main(int argc, char **argv)
{
	type_t meta[LEN(term)];

	if (!valid(LEN(term), term, meta)) {
		puts("term not valid");
		return 1;
	}

	showterm(term, NULL);

	if (!infer(term, meta)) {
		puts("term not well-typed");
		return 1;
	}

	puts("=");
	showterm(term, meta);

	return 0;
}
