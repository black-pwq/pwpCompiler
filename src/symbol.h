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
	bt_char,
	bt_bool,
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
	SimpleSymbol(BType t);
	virtual int getSize() const override;
	virtual void dump(FILE *f) const override;
	virtual void append(const int w) override;
};

struct ArraySymbol : VarSymbol
{
	ArraySymbol(BType b);
	virtual int getSize() const override;
	virtual void dump(FILE *f) const override;
	virtual void append(const int w) override;

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
	FunSymbol(BType r);
	void appendType(VarSymbol *s);
	virtual int getSize() const override;
	virtual void dump(FILE *f) const override;
};