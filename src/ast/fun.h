#pragma once

#include "base.h"
#include "expr.h"
#include "type.h"
#include "stmt.h"

struct FunDef : BaseAST
{
	std::unique_ptr<Type> type;
	std::unique_ptr<Symbol> name;
	std::unique_ptr<FieldList> fields;
	std::unique_ptr<BlockStmt> block;
	FunDef(Type *t, Symbol *n, FieldList *f, BlockStmt *b) : type(t), name(n), fields(f), block(b) {}
	FunDef(Type *t, Symbol *n, BlockStmt *b) : type(t), name(n), fields(new FieldList()), block(b) {}
	void dumpInner(const int i) const override;
	void dump(const int i = 0) const override;
};

struct CompUnit : BaseAST
{
	std::unique_ptr<FunDef> func_def;
	void dumpInner(const int i) const override;
	void dump(const int i = 0) const override;
};
