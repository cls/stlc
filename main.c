#include <stdio.h>
#include "stlc.h"

#define LEN(X) (sizeof (X) / sizeof *(X))

//static T term[] = { -1, 0 };
//static T term[] = { -1, 3, 0, 0 };
static T term[] = { -1, -1, 4, 0, 1 };

int
main(int argc, char **argv)
{
	T weight = weigh(LEN(term), term);

	if (weight == NIL) {
		puts("term not valid");
		return 1;
	}

	showterm(term, NULL);

	T types[weight];

	T type = infer(term, types);

	if (type == NIL) {
		puts("term not well-typed");
		return 1;
	}

	puts("=");
	showterm(term, types);
	puts(":");
	showtype(types, type);

	return 0;
}
