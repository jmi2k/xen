typedef struct Lexer Lexer;
typedef struct Token Token;

enum {
	Eof,
	Name,
	Symbol,
	Eol,
	Open,
	Close,
};

struct Lexer {
	char *src;
	char *p;
	char *p₀;
	char *error;
};

struct Token {
	char *p;
	int len;
	int type;
};

int lex(Lexer *, Token *);
