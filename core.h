typedef struct Core Core;

enum {
	λ,
	Π,
	Σ,
	App,
	□,
	Var,
	Hole,
};

struct Core {
	int type;
	union {
		struct { Slice x; Core *α, *e; } abs;
		struct { Core *l, *r; } op;
		struct { Slice name; int i; } var;
		int □;
	} u;
};

void debruijn(Core *);
