#pragma once
#include "base.h"
#include "expr.h"

struct Stmt: BaseAST {};

struct RetStmt: Stmt {
	std::unique_ptr<Expr> expr;
	RetStmt(Expr *e) : expr(e) {}
	void dumpInner(const int i) const override ;
	void dump(const int i = 0) const override ;
};

struct BlockStmt: Stmt {
	std::vector<std::unique_ptr<Stmt>> stmts;
	BlockStmt(Stmt *s) {stmts.emplace_back(std::unique_ptr<Stmt>(s));}
	void dumpInner(const int i) const override ;
	void dump(const int i = 0) const override ;
};