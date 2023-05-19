#pragma once

#include "base.h"
#include "symbol.h"

struct Type: BaseAST {};

struct SimpleType: Type{
	std::unique_ptr<Symbol> name;
	SimpleType(Symbol *n): name(n) {}
	void dumpInner(const int i) const override;
	void dump(const int i = 0) const override;
};

struct Field: BaseAST {
	std::unique_ptr<Symbol> type;
	std::unique_ptr<Symbol> name;
	Field(Symbol *t, Symbol *n): type(t), name(n) {}
	void dumpInner(const int i) const override;
	void dump(const int i = 0) const override;
};

using FieldList = std::vector<std::unique_ptr<Field>>;

struct RecordType: Type {
	FieldList fields;
};

