typedef struct Abs Abs;
typedef struct Op Op;
typedef struct Ivar Ivar;
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

struct Abs {
	Slice x;
	Core *α;
	Core *e;
};

struct Op {
	Core *l;
	Core *r;
};

struct Ivar {
	Slice name;
	int i;
};

struct Core {
	int type;
	union {
		Abs abs;
		Op op;
		Ivar var;
		int □;
	} u;
};

void debruijn(Core *);
