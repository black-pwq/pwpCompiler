#pragma once
#include "base.h"
#include "symbol.h"
#include "expr.h"

struct Expr;
struct Var: BaseAST {};

struct SimpleVar: Var {
	std::unique_ptr<Symbol> sym;
};

struct FieldVar: Var {
	std::unique_ptr<Var> var;
	std::unique_ptr<Symbol> sym;
};

struct SubscriptVar: Var {
	std::unique_ptr<Var> var;
	std::unique_ptr<Expr> expr;
};