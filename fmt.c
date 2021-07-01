#include <u.h>
#include <libc.h>

#include "misc.h"
#include "core.h"
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
	Core *α, *e;
	Rune op;
	Slice x;

	e = va_arg(f->args, Core *);
	switch(e->type){
	case λ:
		x = e->abs.x;
		if(!x.p){
			x.p = "_";
			x.len = 1;
		}
		α = e->abs.α;
		e = e->abs.e;
		return α->type == Hole
			? fmtprint(f, "(%ς ↦ %ε)", x, e)
			: fmtprint(f, "((%ς:%ε) ↦ %ε)", x, α, e);
	case Π:
	case Σ:
		x = e->abs.x;
		α = e->abs.α;
		op = L"×→"[e->type == Π];
		e = e->abs.e;
		return x.p
			? fmtprint(f, "((%ς:%ε) %C %ε)", x, α, op, e)
			: fmtprint(f, "(%ε %C %ε)", α, op, e);
	case App:
		return fmtprint(f, "(%ε %ε)", e->op.l, e->op.r);
	case □:
		return e->□ == 0
			? fmtprint(f, "⊤")
			: fmtprint(f, "□%ₙ", e->□);
	case Var:
		return e->var.i > 0
			? fmtprint(f, "%ς%ⁿ", e->var.name, e->var.i)
			: fmtprint(f, "%ς", e->var.name);
	case Hole:
		return fmtprint(f, "_");
	default:
		return 0;
	}
}
