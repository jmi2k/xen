#include <u.h>
#include <libc.h>

#include "misc.h"
#include "core.h"
#include "fmt.h"
#include "lex.h"

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
	Lexer L;
	Token t;
	char *src;
	vlong len;
	int fd;

	Core □1 = {□};
	Core α1 = {Var, .u = {.var = {"α", 2}}};
	Core α2 = {Var, .u = {.var = {"α", 2}}};
	Core Π1 = {Π, .u = {.abs = {{nil}, &α1, &α2}}};
	Core λ1 = {λ, .u = {.abs = {{"α", 2}, &□1, &Π1}}};
	Core *E = &λ1;

	argv0 = argv[0];
	if(argc != 2)
		usage();

	fmtinstall(L'ⁿ', nfmt);
	fmtinstall(L'ₙ', nfmt);
	fmtinstall(L'ς', ςfmt);
	fmtinstall(L'ε', εfmt);
	if((fd = open(argv[1], OREAD)) < 0)
		sysfatal("open: %r\n");
	len = seek(fd, 0, 2);
	seek(fd, 0, 0);
	if((src = malloc(len+1)) == nil)
		sysfatal("malloc: %r");
	src[len] = '\0';
	if(readn(fd, src, len) < 0)
		sysfatal("readn: %r");
	print("%s\n", src);
	L.src = src;
	L.p = src;
	while(lex(&L, &t) > 0)
		print("%ς\n", t.span);
	if(L.error)
		sysfatal("lex: %s", L.error);
	print("%ε\n", E);
	debruijn(E);
	print("%ε\n", E);

	free(src);
	exits(nil);
}
