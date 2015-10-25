#include <stdio.h>
#include "stlc.h"

#define LEN(X) (sizeof (X) / sizeof *(X))

/*static term_t term[] = { 3, -1, -1 };*/
/*static term_t term[] = { 5, 1, 4, -1, -1 };*/
static term_t term[] = { 6, 1, 2, 5, -2, -1 };

int
main(void)
{
	type_t *type;

	if (!valid(LEN(term), term)) {
		puts("term not valid");
		return 1;
	}

	showterm(term, NULL, ROOT);
	putchar('\n');

	type = infer(term);

	if (!type) {
		puts("term not well-typed");
		return 1;
	}

	showterm(term, type, ROOT);
	putchar('\n');

	showtype(term, type, TYPEOF(ROOT));
	putchar('\n');

	return 0;
}
