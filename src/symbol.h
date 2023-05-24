#pragma once
#include <string>
#include <memory>
#include <vector>
#include <cassert>

// typedef std::string Symbol;
class SymScope;

enum BType
{
	bt_undef,
	bt_int,
	bt_float,
	bt_void,
	bt_char
};
static int BTypeSize[] = {0, 4, 4, 0};

struct CType
{
	const BType btype;
	const int dim;
};

struct Symbol
{
	// base type for SimpleVar and ArrayVar, return type for FunSymbol
	const BType type;
	int offset = 0;
	SymScope *symScope = nullptr;
	Symbol(BType t) : type(t) {}
	virtual ~Symbol() = default;
	virtual int getSize() const = 0;
	virtual void dump(FILE *f) const = 0;
};

struct VarSymbol : Symbol
{
	/**
	 * length of each dimension measured in WORD
	 * e.g. wordsInDim[0] = 3 for a[3]
	 */
	std::vector<int> wordsInDim;
	VarSymbol(BType t) : Symbol(t) {}
	virtual void append(const int w) = 0;
};

struct SimpleSymbol : VarSymbol
{
	SimpleSymbol(BType t) : VarSymbol(t)
	{
		assert(type != bt_undef);
		assert(type != bt_void);
	}
	virtual int getSize() const override
	{
		return BTypeSize[type];
	}
	virtual void dump(FILE *f) const override
	{
		char tn[][6] = {"undef", "int", "float", "void"};
		fprintf(f, "%s", tn[type]);
	}
	virtual void append(const int w) override
	{
		wordsInDim.push_back(w);
	}
};

struct ArraySymbol : VarSymbol
{
	ArraySymbol(BType b) : VarSymbol(b), sizeInWord(1)
	{
		assert(b != bt_undef);
		assert(b != bt_void);
	}

	virtual int getSize() const override
	{
		return sizeInWord * BTypeSize[type];
	}
	virtual void dump(FILE *f) const override
	{
		char tn[][6] = {"undef", "int", "float", "void"};
		fprintf(f, "%s", tn[type]);
		for (int l : wordsInDim)
			fprintf(f, "[%d]", l);
		fprintf(f, ", sizeInWord = %d, size = %d", sizeInWord, getSize());
	}
	virtual void append(const int w) override
	{
		wordsInDim.push_back(w);
		sizeInWord *= w;
	}

private:
	int sizeInWord;
};

struct FunSymbol : Symbol
{
	/**
	 * Associated VarSymbols with formals.
	 * Those symbols are managed by the symbol table of varaibles.
	*/
	std::vector<VarSymbol *> types;
	FunSymbol(BType r) : Symbol(r) {}
	void appendType(VarSymbol *s) { types.push_back(s); }
	virtual int getSize() const override { return 0; }
	virtual void dump(FILE *f) const override
	{
		char tn[][6] = {"undef", "int", "float", "void"};
		fprintf(f, "returnType = %s ", tn[type]);
		for (auto t : types)
		{
			t->dump(f);
		}
	}
};