#include <u.h>
#include <libc.h>

#include "expr.h"
#include "fmt.h"

void usage(void);
void main(int, char *[]);

void
usage(void)
{
	fprint(2, "usage: %s\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	Expr □1 = {□};
	Expr α1 = {Var, .u = {.var = {"α"}}};
	Expr α2 = {Var, .u = {.var = {"α"}}};
	Expr Π1 = {Π, .u = {.abs = {nil, &α1, &α2}}};
	Expr λ1 = {λ, .u = {.abs = {"α", &□1, &Π1}}};
	Expr *E = &λ1;

	argv0 = argv[0];
	if(argc > 1)
		usage();
	fmtinstall(L'ⁿ', ⁿfmt);
	fmtinstall(L'ₙ', ₙfmt);
	fmtinstall(L'ε', εfmt);
	print("%ε\n", E);
	debruijn(E);
	print("%ε\n", E);
	exits(nil);
}
