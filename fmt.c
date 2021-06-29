#include <u.h>
#include <libc.h>

#include "misc.h"
#include "expr.h"
#include "fmt.h"

int
nfmt(Fmt *f)
{
	Rune buf[256];
	Rune *p, *lut;
	int n;

	n = va_arg(f->args, int);
	p = buf;
	lut = f->r == L'ⁿ'
		? L"⁰¹²³⁴⁵⁶⁷⁸⁹"
		: L"₀₁₂₃₄₅₆₇₈₉";
	runesnprint(buf, sizeof(buf), "%d", n);
	if(*p == L'-')
		*p++ = L'⁻';
	for(; *p; p++)
		*p = lut[*p - '0'];
	return fmtprint(f, "%S", buf);
}

static Rune ops[] = {
	[λ] = L'↦',
	[Π] = L'→',
	[Σ] = L'×',
};

int
ςfmt(Fmt *f)
{
	Slice ς;

	ς = va_arg(f->args, Slice);
	return fmtprint(f, "%.*s", utfnlen(ς.p, ς.len), ς.p);
}

int
εfmt(Fmt *f)
{
	Expr *α, *e;
	Rune op;
	Slice x;

	e = va_arg(f->args, Expr *);
	switch(e->type){
	case λ:
		x = e->u.abs.x;
		if(!x.p){
			x.p = "_";
			x.len = 1;
		}
		α = e->u.abs.α;
		op = ops[e->type];
		e = e->u.abs.e;
		return α->type == Hole
			? fmtprint(f, "(%ς %C %ε)", x, op, e)
			: fmtprint(f, "((%ς:%ε) %C %ε)", x, α, op, e);
	case Π:
	case Σ:
		x = e->u.abs.x;
		α = e->u.abs.α;
		op = ops[e->type];
		e = e->u.abs.e;
		return x.p
			? fmtprint(f, "((%ς:%ε) %C %ε)", x, α, op, e)
			: fmtprint(f, "(%ε %C %ε)", α, op, e);
	case App:
		return fmtprint(f, "(%ε %ε)", e->u.op.l, e->u.op.r);
	case □:
		return e->u.□ == 0
			? fmtprint(f, "⊤")
			: fmtprint(f, "□%ₙ", e->u.□);
	case Var:
		return e->u.var.i > 0
			? fmtprint(f, "%ς%ⁿ", e->u.var.name, e->u.var.i)
			: fmtprint(f, "%ς", e->u.var.name);
	case Hole:
		return fmtprint(f, "_");
	default:
		return 0;
	}
}
