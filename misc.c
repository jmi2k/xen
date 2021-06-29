#include <u.h>
#include <libc.h>

#include "misc.h"

int
strslicecmp(char str[], Slice ς)
{
	int Δ;
	Δ = strncmp(str, ς.p, ς.len);
	if(Δ == 0)
		Δ = str[ς.len];
	return Δ;
}
