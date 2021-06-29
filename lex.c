#include <u.h>
#include <libc.h>

#include "misc.h"
#include "lex.h"

#define Δp(L) (L->p - L->p₀)
#define isnamerune(r) (isalpharune(r) || isdigitrune(r) || r == '_')

#define Reject(L, e) (L->error = e,  -1)
#define Accept(L) {                         \
	L->p += len;                            \
	len = chartorune(&r, L->p);             \
	USED(len);                              \
	if(r == Runeerror)                      \
		return Reject(L, "malformed rune"); \
}

inline int
Ok(Lexer *L, Token *t, int type)
{
	t->span.p = L->p₀;
	t->span.len = Δp(L);
	t->type = type;
	return t->span.len;
}

int
lex(Lexer *L, Token *t)
{
	Rune r;
	int len;

	L->error = nil;
	len = chartorune(&r, L->p);
	if(r == Runeerror)
		return Reject(L, "malformed rune");
	L->p₀ = L->p;
	while(*L->p){
		if(runestrchr(L":⊤→↦×", r)){
			Accept(L);
			return Ok(L, t, Symbol);
		}else if(r == '('){
			Accept(L);
			return Ok(L, t, Open);
		}else if(r == ')'){
			Accept(L);
			return Ok(L, t, Close);
		}else if(isspacerune(r)){
			Accept(L);
			L->p₀ = L->p;
		}else if(isnamerune(r)){
			Accept(L);
			while(isnamerune(r))
				Accept(L);
			return Ok(L, t, Name);
		}else if(r == '\n'){
			Accept(L);
			while(r == '\n')
				Accept(L);
			return Ok(L, t, Eol);
		}else{
			return Reject(L, "unexpected rune");
		}
	}

	return Ok(L, t, Eof);
}
