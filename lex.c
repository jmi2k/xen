#include <u.h>
#include <libc.h>

#include "misc.h"
#include "lex.h"

#define Δp(L) (L->p - L->p₀)
#define isnamerune(r) (isalpharune(r) || isdigitrune(r) || r == '_')

#define reject(L, e) (L->error = e,  -1)
#define accept(L) {                         \
	L->p += len;                            \
	len = chartorune(&r, L->p);             \
	USED(len);                              \
	if(r == Runeerror)                      \
		return reject(L, "malformed rune"); \
}

inline int
token(Lexer *L, Token *t, int type)
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
		return reject(L, "malformed rune");
	L->p₀ = L->p;
	while(*L->p){
		if(runestrchr(L":⊤→↦×", r)){
			accept(L);
			return token(L, t, Symbol);
		}else if(r == '('){
			accept(L);
			return token(L, t, Open);
		}else if(r == ')'){
			accept(L);
			return token(L, t, Close);
		}else if(isspacerune(r)){
			accept(L);
			L->p₀ = L->p;
		}else if(isnamerune(r)){
			accept(L);
			while(isnamerune(r))
				accept(L);
			return token(L, t, Name);
		}else if(r == '\n'){
			accept(L);
			while(r == '\n')
				accept(L);
			return token(L, t, Eol);
		}else{
			return reject(L, "unexpected rune");
		}
	}

	return token(L, t, Eof);
}
