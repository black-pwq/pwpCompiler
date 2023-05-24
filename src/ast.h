#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include "symbol.h"
#include "symtab.h"
#include "llvm.h"
extern int yylineno;

struct CompUnit;
struct Type;

struct Expr;
struct UniExpr;
struct BiExpr;
struct Assign;
struct Call;
struct Var;
struct SimpleVar;
struct ArrayVar;

struct Item;
struct Stmt;
struct ExprList;

struct VarDef;
struct InitVarDef;
struct InitArrayDef;

struct Unit;
struct Decl;
struct VarDecl;
struct FunDef;

struct BaseAST
{
	int lineno;
	static int errid;
	static SymTab varSt;
	static SymTab funSt;
	BaseAST() : lineno(yylineno) {}
	virtual ~BaseAST() = default;
	void dump(const int i = 0) const;
	virtual int typeCheck() const {return 0;}

protected:
	void indent(const int i) const;
	virtual void dumpInner(const int i) const;
};

// CompUnit
struct CompUnit : BaseAST
{
	int lineno;
	std::vector<std::unique_ptr<Unit>> units;
	CompUnit() = default;
	CompUnit(Unit *u) {units.emplace_back(std::unique_ptr<Unit>(u));}
	void append(Unit *u) {units.emplace_back(std::unique_ptr<Unit>(u));}
	virtual int typeCheck() const override;
    void codegen();
protected:
    void dumpInner(const int i) const override;
};

// Type
struct Type : BaseAST {
	const BType btype;
	int dim = 0;
	Type(BType b) : btype(b) {}
	void addDim() {++dim;}
protected:
	void dumpInner(const int i) const override; 
};

struct Field : BaseAST
{
	std::unique_ptr<Type> type;
	std::unique_ptr<Var> name;
	Field(Type *t, Var *n) : type(t), name(n) {}
	virtual int typeCheck() const override;

protected:
	void dumpInner(const int i) const override;
};
// no codegen
using FieldList = std::vector<std::unique_ptr<Field>>;

// Expr
struct Expr: BaseAST {
	bool evaluable;
	float num;
	Expr() : evaluable(false), num(0) {}
	Expr(float n) : evaluable(true), num(n) {}
	llvm::Value *codegen();
protected:
	virtual void dumpInner(const int i) const override;
    
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
	UniExpr(UniOp o, Expr *e); 
    llvm::Value *codegen();
protected:
	virtual void dumpInner(const int i) const override;
};

// Value codegen
struct BiExpr : Expr
{
	std::shared_ptr<Expr> left; // shared_ptr for binary assignment
	BiOp op;
	std::shared_ptr<Expr> right;
	BiExpr(Expr *l, BiOp o, Expr *r) ;
	BiExpr(std::shared_ptr<Expr> l, BiOp o, Expr *r) ;
	BiExpr(Expr *l, BiOp o, std::shared_ptr<Expr> r);
	BiExpr(std::shared_ptr<Expr> l, BiOp o, std::shared_ptr<Expr> r) ;
    llvm::Value *codegen();

    void dumpInner(const int i) const;

protected:
    // void dumpInner(const int i) const override;
};

struct StringExpr : Expr
{
	std::unique_ptr<std::string> str;
	StringExpr(std::string *s) : str(s) {}
protected:
	virtual void dumpInner(const int i) const override;
};

struct ConstExpr : Expr {
	ConstExpr(float n) : Expr(n) {}
};

struct IntExpr : ConstExpr {
	IntExpr(const int i) : ConstExpr(i) {}
protected:
	void dumpInner(const int i) const override ;
};

struct FloatExpr : ConstExpr {
	FloatExpr(const float f) : ConstExpr(f) {}
protected:
	void dumpInner(const int i) const override ;
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
	ExprList(Expr *e) {list.emplace_back(std::unique_ptr<Expr>(e));}
	void append(Expr *e) {list.emplace_back(std::unique_ptr<Expr>(e));}

	/**
	 * Perform symbol lookup. Declarations are done by Decl class.
	*/
	virtual int typeCheck() const override;
    llvm::Value *codegen();

protected:
    void dumpInner(const int i) const override
	{
		for (auto &e : list)
			e->dump(i);
	}
};

struct Call : Expr {
	std::unique_ptr<std::string> name;
	std::unique_ptr<ExprList> params;
	Call(std::string *n, ExprList *p) : name(n), params(p) {}
	virtual int typeCheck() const override;
    llvm::Value *codegen();
};

// block
struct If : Stmt
{
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Stmt> stmt;
	If(Expr *e, Stmt *s) : expr(e), stmt(s) {}
	virtual int typeCheck() const override;
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
	virtual int typeCheck() const override;
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
	virtual int typeCheck() const override;
};

// block
struct For : Stmt
{
	std::unique_ptr<Expr> init;
	std::unique_ptr<Expr> expr;
	std::unique_ptr<Expr> tail;
	std::unique_ptr<Stmt> body;
	For(Expr *i, Expr *e, Expr *t, Stmt *b) : init(i), expr(e), tail(t), body(b) {}
	virtual int typeCheck() const override;

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
	virtual int typeCheck() const override;

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
	void append(Item *s) {items.emplace_back(std::unique_ptr<Item>(s));}
	virtual int typeCheck() const override;

protected:
	void dumpInner(const int i) const override;
};

// Var
struct Var: Expr { 
	std::unique_ptr<std::string> sym;
	std::unique_ptr<ExprList> exprs;
	Var(std::string *s, ExprList *e) : sym(s), exprs(e) {}
	virtual int typeCheck() const = 0;
	// virtual int sizeInWord() const = 0;
protected:
	virtual void dumpInner(const int i) const override;
};

struct SimpleVar: Var {
	SimpleVar(std::string *s) : Var(s, new ExprList()) {}

	/**
	 * TypeCheck will not be called if it is in VarDef.
	 * Here, we are looking for the symbol in the symTab.
	*/
	virtual int typeCheck() const override;
	// virtual int sizeInWord() const {return 1;}
	// dumpInner inherited from Var
};

struct ArrayVar: Var {
	ArrayVar(std::string *v, ExprList *e) : Var(v, e) {}

	/**
	 * TypeCheck will not be called if it is in VarDef.
	 * Here, we are looking for the symbol in the symTab.
	*/
	virtual int typeCheck() const override;
	// virtual int sizeInWord() const override;
protected:
	virtual void dumpInner(const int i) const override;
};

struct VarDef : BaseAST
{
	std::unique_ptr<Var> name;
	VarDef(Var *n) : name(n) {}
	// No need to implement typeCheck here, since the type is not associcated in this class.
	// We check and insert symbols in VarDecl
protected:
	virtual void dumpInner(const int i) const override;
};

using VarDefs = std::vector<std::unique_ptr<VarDef>>;

struct InitVarDef : VarDef {
	std::unique_ptr<Expr> expr;
	InitVarDef(Var *n, Expr *e) : VarDef(n), expr(e) {}
	// typeCheck inherited from VarDef
};

struct InitArrayDef : VarDef {
	std::unique_ptr<ExprList> exprs;
	InitArrayDef(Var *n, ExprList *e) : VarDef(n), exprs(e) {}
	// typeCheck inherited from VarDef
};

struct Assign : Expr
{
	std::shared_ptr<Var> var; // shared pointer for binary assignment
	std::unique_ptr<Expr> expr;
	Assign(Var *v, Expr *e) : var(v), expr(e) {}
	Assign(std::shared_ptr<Var> v, Expr *e) : var(v), expr(e) {}
	virtual int typeCheck() const override;
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
	void append(VarDef *v) {vars->emplace_back(std::unique_ptr<VarDef>(v));}
	virtual int typeCheck() const override;
protected:
	virtual void dumpInner(const int i) const override;
};

// Fun
//
struct FunDef : Unit
{
	std::unique_ptr<Type> type;
	std::unique_ptr<std::string> name;
	std::unique_ptr<FieldList> fields;
	std::unique_ptr<Block> block;
	FunDef(Type *t, std::string *n, FieldList *f, Block *b) : type(t), name(n), fields(f), block(b) {}
	FunDef(Type *t, std::string *n, Block *b) : type(t), name(n), fields(new FieldList()), block(b) {}
    void codegen();
	virtual int typeCheck() const override;
protected:
    void dumpInner(const int i) const override;
};

