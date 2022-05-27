enum Type {
    Function,
    Integer,
    Float,
    Character,
    String,
    Bool,
    Void,
    ContextSeparator
};

struct Variable {
	int reg;
	enum Type type;
};