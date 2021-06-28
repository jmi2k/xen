#include <u.h>
#include <libc.h>

#include "expr.h"

typedef struct Bind Bind;

struct Bind {
	Bind *prev;
	char *name;
};

static int
lookup(Bind *Γ, char name[])
{
	int i;

	for(i = 1; Γ; i++){
		if(strcmp(Γ->name, name) == 0)
			return i;
		Γ = Γ->prev;
	}

	return 0;
}

static void
_debruijn(Expr *e, Bind *Γ)
{
	Bind bind;

	switch(e->type){
	case λ:
	case Π:
	case Σ:
		_debruijn(e->u.abs.α, Γ);
		if(e->u.abs.x){
			bind.prev = Γ;
			bind.name = e->u.abs.x;
			Γ = &bind;
		}
		_debruijn(e->u.abs.e, Γ);
		break;
	case App:
		_debruijn(e->u.op.l, Γ);
		_debruijn(e->u.op.r, Γ);
		break;
	case Var:
		e->u.var.i = lookup(Γ, e->u.var.name);
		break;
	}
}

void
debruijn(Expr *e)
{
	_debruijn(e, nil);
}
