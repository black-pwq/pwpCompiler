#pragma once
#include "base.h"
#include "var.h"

struct Expr: BaseAST {};

enum BiOp {add, sub, times, divide, eq, neq, lt, le, gt, ge};
enum UniOp {plus, minus, neg};

struct UniExpr: Expr {
	std::unique_ptr<Expr> expr;
	UniOp op;
};

struct BiExpr: Expr {
	std::unique_ptr<Expr> left, right;
	BiOp op;
};

struct VarExpr: Expr {
	std::unique_ptr<Var> var;
};

struct IntExpr: Expr {
	const int num;
	IntExpr(const int n) : num(n) {}
	void dump(const int i = 0) const override;
	void dumpInner(const int i) const override;
};

struct FloatExpr: Expr {
	const double num;
	FloatExpr(const double n) : num(n) {}
};

struct ExprList: Expr {
	std::unique_ptr<std::vector<Expr>> list;
};
