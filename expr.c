#include <u.h>
#include <libc.h>

#include "misc.h"
#include "expr.h"

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
		if(strncmp(Γ->name, name.p, name.len) == 0)
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
		if(e->u.abs.x.p){
			bind.prev = Γ;
			bind.name = e->u.abs.x.p;
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
