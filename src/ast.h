#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include "symbol.h"
#include "../llvm.h"

extern int yylineno;

struct BaseAST
{
	int lineno;
	BaseAST() : lineno(yylineno) {}
	virtual ~BaseAST() = default;
	void dump(const int i = 0) const;
	virtual llvm::Value *codegen() = 0;

protected:
	void indent(const int i) const;
	virtual void dumpInner(const int i) const;
};

struct Var;

// Type
struct Type : BaseAST
{
};

struct SimpleType : Type
{
	std::unique_ptr<Symbol> name;
	SimpleType(Symbol *n) : name(n) {}
	// virtual llvm::Value *codegen() override;

protected:
	void dumpInner(const int i) const override;
};

struct Field : BaseAST
{
	std::unique_ptr<Symbol> type;
	std::unique_ptr<Var> name;
	Field(Symbol *t, Var *n) : type(t), name(n) {}
	virtual llvm::Value *codegen() override;

protected:
	void dumpInner(const int i) const override;
};

using FieldList = std::vector<std::unique_ptr<Field>>;

// Expr
struct Expr : BaseAST
{
	virtual llvm::Value *codegen() override;

protected:
	void dumpInner(const int i) const { BaseAST::dumpInner(i); }
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

struct UniExpr : Expr
{
	UniOp op;
	std::unique_ptr<Expr> expr;
	UniExpr(UniOp o, Expr *e) : op(o), expr(e) {}
	virtual llvm::Value *codegen() override;
};

struct BiExpr : Expr
{
	std::unique_ptr<Expr> left;
	BiOp op;
	std::unique_ptr<Expr> right;
	BiExpr(Expr *l, BiOp o, Expr *r) : left(l), op(o), right(r) {}
	virtual llvm::Value * codegen() override;

};

template <typename T>

struct NumExpr : Expr
{
	const T num;
	NumExpr(const T n) : num(n) {}
	virtual llvm::Value * codegen() override;

protected:
	void dumpInner(const int i) const override
	{
		indent(i);
		std::cout << num << std::endl;
	}
};

struct ExprList : Expr
{
	std::vector<std::unique_ptr<Expr>> list;
	ExprList(Expr *e) { list.emplace_back(std::unique_ptr<Expr>(e)); }
	virtual llvm::Value * codegen() override;

};

// struct AddressExpr : Expr {
// 	std::unique_ptr<Var> var;
// 	AddressExpr(Var *v) : var(v) {}
// };

// Stmt
struct Stmt : BaseAST
{
};

struct Assign : Stmt
{
	std::unique_ptr<Var> var;
	std::unique_ptr<Expr> expr;
	Assign(Var *v, Expr *e) : var(v), expr(e) {}
	virtual llvm::Value * codegen() override;

};

struct If : Stmt
{
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> stmt;
	If(Expr *e, Stmt *s) : expr(e), stmt(s) {}
	virtual llvm::Value * codegen() override;

};

struct IfElse : Stmt
{
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> ts;
	std::unique_ptr<Stmt> fs;
	IfElse(Expr *e, Stmt *t, Stmt *f) : expr(e), ts(t), fs(f) {}
	virtual llvm::Value * codegen() override;

};

struct While : Stmt
{
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> stmt;
	While(Expr *e, Stmt *s) : expr(e), stmt(s) {}
	virtual llvm::Value * codegen() override;

};

struct For : Stmt
{
	std::unique_ptr<Stmt> init;
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> tail;
	std::unique_ptr<Stmt> body;
	For(Stmt *i, Expr *e, Stmt *t, Stmt *b) : init(i), expr(e), tail(t), body(b) {}
	virtual llvm::Value * codegen() override;

};

struct Return : Stmt
{
	std::unique_ptr<Expr> expr;
	Return(Expr *e) : expr(e) {}
	virtual llvm::Value * codegen() override;


protected:
	void dumpInner(const int i) const override;
};

struct Break : Stmt
{
};
struct Continue : Stmt
{
};

using StmtList = std::vector<std::unique_ptr<Stmt>>;

struct Block : Stmt
{
	StmtList stmts;
	Block() = default;
	Block(Stmt *s) { stmts.emplace_back(std::unique_ptr<Stmt>(s)); }
	void append(Stmt *s) { stmts.emplace_back(std::unique_ptr<Stmt>(s)); }
	virtual llvm::Value * codegen() override;

protected:
	void dumpInner(const int i) const override;
};

// Var
struct Var : Expr
{
	virtual llvm::Value *codegen() override;

protected:
	void dumpInner(const int i) const { Expr::dumpInner(i); }
};

struct SimpleVar : Var
{
	std::unique_ptr<Symbol> sym;
	SimpleVar(Symbol *s) : sym(s) {}
	virtual llvm::Value *codegen() override;

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
	virtual llvm::Value *codegen() override;
};

// Unit
struct Unit : BaseAST
{
};

struct Decl : Unit
{
};

struct VarDecl : Decl
{
	std::unique_ptr<Symbol> type;
	std::unique_ptr<Var> var;
	VarDecl(Symbol *t, Var *v) : type(t), var(v) {}
	virtual llvm::Value *codegen() override;
};

// Fun
struct FunDef : Unit
{
	std::unique_ptr<Type> type;
	std::unique_ptr<Symbol> name;
	std::unique_ptr<FieldList> fields;
	std::unique_ptr<Block> block;
	FunDef(Type *t, Symbol *n, FieldList *f, Block *b) : type(t), name(n), fields(f), block(b) {}
	FunDef(Type *t, Symbol *n, Block *b) : type(t), name(n), fields(new FieldList()), block(b) {}
	llvm::Function *codegen() ;

protected:
	void dumpInner(const int i) const override;
};

// CompUnit
struct CompUnit : BaseAST
{
	std::vector<std::unique_ptr<Unit>> units;
	CompUnit(Unit *u) { units.emplace_back(std::unique_ptr<Unit>(u)); }
	void append(Unit *u) { units.emplace_back(std::unique_ptr<Unit>(u)); }
	virtual llvm::Value *codegen() override;

protected:
	void dumpInner(const int i) const override;
};

template <typename T>
inline llvm::Value *NumExpr<T>::codegen()
{
	if(T == int ){
		return llvm::ConstantInt::get();
	}
    return nullptr;
}
