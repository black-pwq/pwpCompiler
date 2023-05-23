#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include "symbol.h"
#include "llvm.h"
extern int yylineno;

// no instance
struct BaseAST
{
	int lineno;
	BaseAST() : lineno(yylineno) {}
	virtual ~BaseAST() = default;
	void dump(const int i = 0) const;
	
protected:
	void indent(const int i) const;
	virtual void dumpInner(const int i) const;
};

struct Var;

// no codegen
struct Type : BaseAST
{
	std::unique_ptr<Symbol> name;
	Type(Symbol *n) : name(n) {}

protected:
	void dumpInner(const int i) const override;
};

// no codegen
struct Field : BaseAST
{
	std::unique_ptr<Type> type;
	std::unique_ptr<Var> name;
	Field(Type *t, Var *n) : type(t), name(n) {}

protected:
	void dumpInner(const int i) const override;
};
// no codegen
using FieldList = std::vector<std::unique_ptr<Field>>;

// Expr
// Value codegen
struct Expr : BaseAST
{
	virtual llvm::Value *codegen();
};

enum BiOp
{
	bi_add,
	bi_sub,
	bi_times,
	bi_divide,
	bi_and,
	bi_or,
	bi_eq,
	bi_neq,
	bi_lt,
	bi_le,
	bi_gt,
	bi_ge
};
enum UniOp
{
	uni_plus,
	uni_minus,
	uni_not
};

// Value codegen
struct UniExpr : Expr
{
	UniOp op;
	std::unique_ptr<Expr> expr;
	UniExpr(UniOp o, Expr *e) : op(o), expr(e) {}
	virtual llvm::Value *codegen() override;
};

// Value codegen
struct BiExpr : Expr
{
	std::shared_ptr<Expr> left; // shared_ptr for binary assignment
	BiOp op;
	std::shared_ptr<Expr> right;
	BiExpr(Expr *l, BiOp o, Expr *r) : left(l), op(o), right(r) {}
	virtual llvm::Value *codegen() override;

	BiExpr(std::shared_ptr<Expr> l, BiOp o, Expr *r) : left(l), op(o), right(r) {}
	BiExpr(Expr *l, BiOp o, std::shared_ptr<Expr> r) : left(l), op(o), right(r) {}
	BiExpr(std::shared_ptr<Expr> l, BiOp o, std::shared_ptr<Expr> r) : left(l), op(o), right(r) {}

protected:
	// void dumpInner(const int i) const override;
};

struct StringExpr : Expr
{
	std::unique_ptr<std::string> str;
	StringExpr(std::string *s) : str(s) {}
};

// Value codegen
template <typename T>
struct NumExpr : Expr
{
	const T num;
	NumExpr(const T n) : num(n) {}
	virtual llvm::Value *codegen() override;

protected:
	void dumpInner(const int i) const override
	{
		indent(i);
		std::cout << num << std::endl;
	}
};

// Stmt
struct Item : BaseAST
{
};

struct Stmt : Item
{
};

// value codegen of the last expr
struct ExprList : Expr, Stmt
{
	std::vector<std::unique_ptr<Expr>> list;
	ExprList() = default;
	ExprList(Expr *e) { list.emplace_back(std::unique_ptr<Expr>(e)); }
	// ExprList() = default;
	void append(Expr *e) { list.emplace_back(std::unique_ptr<Expr>(e)); }
	virtual llvm::Value *codegen() override;

protected:
	void dumpInner(const int i) const override
	{
		for (auto &e : list)
			e->dump(i);
	}
};

struct Call : Expr
{
	std::unique_ptr<Symbol> name;
	std::unique_ptr<ExprList> params;
	Call(Symbol *n, ExprList *p) : name(n), params(p) {}
	virtual llvm::Value *codegen() override;
};

// block
struct If : Stmt
{
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> stmt;
	If(Expr *e, Stmt *s) : expr(e), stmt(s) {}

protected:
	void dumpInner(const int i) const override
	{
		expr->dump(i);
		stmt->dump(i);
	}
};

// block
struct IfElse : Stmt
{
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> ts;
	std::unique_ptr<Stmt> fs;
	IfElse(Expr *e, Stmt *t, Stmt *f) : expr(e), ts(t), fs(f) {}

protected:
	void dumpInner(const int i) const override
	{
		expr->dump(i);
		ts->dump(i);
		fs->dump(i);
	}
};

// block
struct While : Stmt
{
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> stmt;
	While(Expr *e, Stmt *s) : expr(e), stmt(s) {}
};

// block
struct For : Stmt
{
	std::unique_ptr<Expr> init;
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Expr> tail;
	std::unique_ptr<Stmt> body;
	For(Expr *i, Expr *e, Expr *t, Stmt *b) : init(i), expr(e), tail(t), body(b) {}

protected:
	void dumpInner(const int i) const override
	{
		init->dump(i);
		expr->dump(i);
		tail->dump(i);
		body->dump(i);
	}
};

struct Return : Stmt
{
	std::unique_ptr<Expr> expr;
	Return(Expr *e) : expr(e) {}

protected:
	void dumpInner(const int i) const override;
};

struct Break : Stmt
{
};
struct Continue : Stmt
{
};

using ItemList = std::vector<std::unique_ptr<Item>>;

struct Block : Stmt
{
	ItemList items;
	Block() = default;
	Block(Item *s) { append(s); }
	void append(Item *s) { items.emplace_back(std::unique_ptr<Item>(s)); }

protected:
	void dumpInner(const int i) const override;
};

// Var
struct Var : Expr
{
};

struct SimpleVar : Var
{
	std::unique_ptr<Symbol> sym;
	SimpleVar(Symbol *s) : sym(s) {}

protected:
	void dumpInner(const int i) const override
	{
		indent(i);
		std::cout << *sym << std::endl;
	}
};

struct ArrayVar : Var
{
	std::unique_ptr<Var> var;
	std::unique_ptr<Expr> expr;
	ArrayVar(Var *v, Expr *e) : var(v), expr(e) {}
};

struct VarDef : BaseAST
{
	std::unique_ptr<Var> name;
	VarDef(Var *n) : name(n) {}
};

using VarDefs = std::vector<std::unique_ptr<VarDef>>;

struct InitVarDef : VarDef
{
	std::unique_ptr<Var> name;
	std::unique_ptr<Expr> expr;
	InitVarDef(Var *n, Expr *e) : VarDef(n), expr(e) {}
};

struct InitArrayDef : VarDef
{
	std::unique_ptr<Var> name;
	std::unique_ptr<ExprList> exprs;
	InitArrayDef(Var *n, ExprList *e) : VarDef(n), exprs(e) {}
};

struct Assign : Expr
{
	std::shared_ptr<Var> var; // shared pointer for binary assignment
	std::unique_ptr<Expr> expr;
	Assign(Var *v, Expr *e) : var(v), expr(e) {}
	Assign(std::shared_ptr<Var> v, Expr *e) : var(v), expr(e) {}

protected:
	void dumpInner(const int i) const override
	{
		var->dump(i);
		expr->dump(i);
	}
};

// Unit
struct Unit : BaseAST
{
	virtual void codegen();
};

struct Decl : Unit, Item
{
};

struct VarDecl : Decl
{
	std::unique_ptr<Type> type;
	std::unique_ptr<VarDefs> vars;
	VarDecl(Type *t, VarDefs *v) : type(t), vars(v) {}
	void append(VarDef *v) { vars->emplace_back(std::unique_ptr<VarDef>(v)); }
};

// Fun
//
struct FunDef : Unit
{
	std::unique_ptr<Type> type;
	std::unique_ptr<Symbol> name;
	std::unique_ptr<FieldList> fields;
	std::unique_ptr<Block> block;
	FunDef(Type *t, Symbol *n, FieldList *f, Block *b) : type(t), name(n), fields(f), block(b) {}
	FunDef(Type *t, Symbol *n, Block *b) : type(t), name(n), fields(new FieldList()), block(b) {}
	void codegen() override;

protected:
	void dumpInner(const int i) const override;
};

// CompUnit
// codegen void

struct CompUnit : BaseAST
{
	int lineno;
	std::vector<std::unique_ptr<Unit>> units;
	CompUnit() = default;
	CompUnit(Unit *u) { units.emplace_back(std::unique_ptr<Unit>(u)); }
	void append(Unit *u) { units.emplace_back(std::unique_ptr<Unit>(u)); }
	void codegen();

protected:
	void dumpInner(const int i) const override;
};

template <typename T>
inline llvm::Value *NumExpr<T>::codegen()
{
	// if ((T) == "int")
	// {
	// 	// return
	// }
	// else if ((*type) == "float")
	// {
	// }
	return nullptr;
}
