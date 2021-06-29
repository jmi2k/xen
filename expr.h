typedef struct Abs Abs;
typedef struct Op Op;
typedef struct Ivar Ivar;
typedef struct Expr Expr;

enum {
	λ,
	Π,
	Σ,
	App,
	□,
	Var,
	Hole,
};

struct Abs {
	Slice x;
	Expr *α;
	Expr *e;
};

struct Op {
	Expr *l;
	Expr *r;
};

struct Ivar {
	Slice name;
	int i;
};

struct Expr {
	int type;
	union {
		Abs abs;
		Op op;
		Ivar var;
		int □;
	} u;
};

void debruijn(Expr *);
