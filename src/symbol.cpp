#include "symbol.h"

static int BTypeSize[] = {0, 4, 4, 0};
static char tn[][6] = {"undef", "int", "float", "void", "char", "bool"};

SimpleSymbol::SimpleSymbol(BType t) : VarSymbol(t)
{
	assert(type != bt_undef);
	assert(type != bt_void);
}
int SimpleSymbol::getSize() const
{
	return BTypeSize[type];
}
void SimpleSymbol::dump(FILE *f) const
{
	fprintf(f, "%s", tn[type]);
}
void SimpleSymbol::append(const int w)
{
	wordsInDim.push_back(w);
}

ArraySymbol::ArraySymbol(BType b) : VarSymbol(b), sizeInWord(1)
{
	assert(b != bt_undef);
	assert(b != bt_void);
}

ArraySymbol::ArraySymbol(ArraySymbol &other) : ArraySymbol(other.type) {
	for(auto e : other.wordsInDim)
		append(e);
}

ArraySymbol::ArraySymbol(ArraySymbol *other) : ArraySymbol(other->type) {
	for(auto e : other->wordsInDim)
		append(e);
}

int ArraySymbol::getSize() const
{
	return sizeInWord * BTypeSize[type];
}
void ArraySymbol::dump(FILE *f) const
{
	fprintf(f, "%s", tn[type]);
	for (int l : wordsInDim)
		fprintf(f, "[%d]", l);
	fprintf(f, ", sizeInWord = %d, size = %d ", sizeInWord, getSize());
}
void ArraySymbol::append(const int w)
{
	wordsInDim.push_back(w);
	sizeInWord *= w;
}

FunSymbol::FunSymbol(BType r) : Symbol(r) {}
void FunSymbol::appendType(VarSymbol *s) { types.push_back(s); }
int FunSymbol::getSize() const { return 0; }
void FunSymbol::dump(FILE *f) const
{
	fprintf(f, "returnType = %s ", tn[type]);
	int i = 0;
	for (auto t : types)
	{
		fprintf(f, "{%d}", ++i);
		t->dump(f);
	}
}