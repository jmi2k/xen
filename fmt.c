#include <u.h>
#include <libc.h>

#include "expr.h"
#include "fmt.h"

int
ⁿfmt(Fmt *f)
{
	Rune buf[256];
	Rune *p;
	int n;

	n = va_arg(f->args, int);
	p = buf;
	runesnprint(buf, sizeof(buf), "%d", n);
	if(*p == L'-')
		*p++ = L'⁻';
	for(; *p; p++)
		*p = L"⁰¹²³⁴⁵⁶⁷⁸⁹"[*p - '0'];
	return fmtprint(f, "%S", buf);
}

int
ₙfmt(Fmt *f)
{
	Rune buf[256];
	Rune *p;
	int n;

	n = va_arg(f->args, int);
	p = buf;
	runesnprint(buf, sizeof(buf), "%d", n);
	if(*p == L'-')
		*p++ = L'₋';
	for(; *p; p++)
		*p = L"₀₁₂₃₄₅₆₇₈₉"[*p - '0'];
	return fmtprint(f, "%S", buf);
}

static Rune ops[] = {
	[λ] = L'↦',
	[Π] = L'→',
	[Σ] = L'×',
};

int
εfmt(Fmt *f)
{
	Expr *α, *e;
	Rune op;
	char *x;

	e = va_arg(f->args, Expr *);
	switch(e->type){
	case λ:
		x = e->u.abs.x
			? e->u.abs.x
			: "_";
		α = e->u.abs.α;
		op = ops[e->type];
		e = e->u.abs.e;
		return α->type == Hole
			? fmtprint(f, "(%s %C %ε)", x, op, e)
			: fmtprint(f, "((%s:%ε) %C %ε)", x, α, op, e);
	case Π:
	case Σ:
		x = e->u.abs.x;
		α = e->u.abs.α;
		op = ops[e->type];
		e = e->u.abs.e;
		return x
			? fmtprint(f, "((%s:%ε) %C %ε)", x, α, op, e)
			: fmtprint(f, "(%ε %C %ε)", α, op, e);
	case App:
		return fmtprint(f, "(%ε %ε)", e->u.op.l, e->u.op.r);
	case □:
		return e->u.□ == 0
			? fmtprint(f, "⊤")
			: fmtprint(f, "□%ₙ", e->u.□);
	case Var:
		return e->u.var.i > 0
			? fmtprint(f, "%s%ⁿ", e->u.var.name, e->u.var.i)
			: fmtprint(f, "%s", e->u.var.name);
	case Hole:
		return fmtprint(f, "_");
	default:
		return 0;
	}
}
