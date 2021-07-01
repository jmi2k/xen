#include <u.h>
#include <libc.h>

#include "misc.h"
#include "core.h"

typedef struct Bind Bind;

struct Bind {
	Bind *prev;
	char *name;
};

static int
lookup(Bind *Γ, Slice name)
{
	int i;

	for(i = 1; Γ; i++){
		if(strslicecmp(Γ->name, name) == 0)
			return i;
		Γ = Γ->prev;
	}

	return 0;
}

static void
_debruijn(Core *e, Bind *Γ)
{
	Bind bind;

	switch(e->type){
	case λ:
	case Π:
	case Σ:
		_debruijn(e->abs.α, Γ);
		if(e->abs.x.p){
			bind.prev = Γ;
			bind.name = e->abs.x.p;
			Γ = &bind;
		}
		_debruijn(e->abs.e, Γ);
		break;
	case App:
		_debruijn(e->op.l, Γ);
		_debruijn(e->op.r, Γ);
		break;
	case Var:
		e->var.i = lookup(Γ, e->var.name);
		break;
	}
}

void
debruijn(Core *e)
{
	_debruijn(e, nil);
}
