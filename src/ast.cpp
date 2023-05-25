#include "ast.h"
using namespace std;

int BaseAST::errid;
SymTab BaseAST::varSt;
SymTab BaseAST::funSt;
int Expr::t;

static float eval_bi(int l, BiOp op, int r)
{
	switch (op)
	{
	case BiOp::bi_add:
		return l + r;
	case BiOp::bi_sub:
		return l - r;
	case BiOp::bi_times:
		return l * r;
	case BiOp::bi_divide:
		return l / r;
	case BiOp::bi_eq:
		return l == r;
	case BiOp::bi_neq:
		return l != r;
	case BiOp::bi_lt:
		return l < r;
	case BiOp::bi_gt:
		return l > r;
	case BiOp::bi_le:
		return l <= r;
	case BiOp::bi_ge:
		return l >= r;
	case BiOp::bi_or:
		return l || r;
	case BiOp::bi_and:
		return l && r;
	default:
		assert(false);
	}
}

BiExpr::BiExpr(Expr *l, BiOp o, Expr *r) : left(l), op(o), right(r)
{
	evaluable = l->evaluable && r->evaluable;
	if (evaluable)
		num = eval_bi(left->num, o, right->num);
}
BiExpr::BiExpr(std::shared_ptr<Expr> l, BiOp o, Expr *r) : left(l), op(o), right(r)
{
	evaluable = l->evaluable && r->evaluable;
	if (evaluable)
		num = eval_bi(left->num, o, right->num);
}
BiExpr::BiExpr(Expr *l, BiOp o, std::shared_ptr<Expr> r) : left(l), op(o), right(r)
{
	evaluable = l->evaluable && r->evaluable;
	if (evaluable)
		num = eval_bi(left->num, o, right->num);
}
BiExpr::BiExpr(std::shared_ptr<Expr> l, BiOp o, std::shared_ptr<Expr> r) : left(l), op(o), right(r)
{
	evaluable = l->evaluable && r->evaluable;
	if (evaluable)
		num = eval_bi(left->num, o, right->num);
}

UniExpr::UniExpr(UniOp o, Expr *e) : op(o), expr(e)
{
	evaluable = expr->evaluable;
	if (evaluable)
		switch (op)
		{
		case UniOp::uni_minus:
			num = -expr->num;
			break;
		case UniOp::uni_not:
			num = !expr->num;
			break;
		case UniOp::uni_plus:
			break;
		default:
			assert(false);
		}
}

void BaseAST::dump(const int i) const
{
	indent(i);
	const char *name = abi::__cxa_demangle(typeid(*this).name(), NULL, NULL, NULL);
	std::cout << name
			  << " (#line " << lineno << ") "
			  << "{" << std::endl;
	dumpInner(i + 1);
	indent(i);
	std::cout << "}    $" << name << std::endl;
}

void BaseAST::dumpInner(const int i) const
{
	indent(i);
	std::cout << "NONPRINTABLE" << std::endl;
}

void BaseAST::indent(const int i) const
{
	for (int j = 0; j < i; j++)
		std::cout << "  ";
}

void CompUnit::dumpInner(const int i) const
{
	for (auto &u : units)
		u->dump(i);
}

void FunDef::dumpInner(const int i) const
{
	type->dump(i);
	indent(i);
	cout << *name << endl;
	for (auto &f : *fields)
		f->dump(i);
	block->dump(i);
}

void Return::dumpInner(const int i) const
{
	expr->dump(i);
}

void Block::dumpInner(const int i) const
{
	for (auto &s : items)
		s->dump(i);
}

void Field::dumpInner(const int i) const
{
	type->dump(i);
	name->dump(i);
}

void VarDecl::dumpInner(const int i) const
{
	type->dump(i);
	for (auto &vardef : *vars)
	{
		vardef->dump(i);
	}
}

void VarDef::dumpInner(const int i) const
{
	name->dump(i);
}

void Var::dumpInner(const int i) const
{
	indent(i);
	cout << *sym << endl;
}

void ArrayVar::dumpInner(const int i) const
{
	indent(i);
	cout << *sym << endl;
	for (auto &e : exprs->list)
	{
		e->dump(i);
	}
}

void Expr::dumpInner(const int i) const
{
	indent(i);
	cout << "eval: " << evaluable << endl;
}

void UniExpr::dumpInner(const int i) const
{
	Expr::dumpInner(i);
	char n[] = {'+', '-', '!'};
	indent(i);
	cout << n[op] << endl;
	expr->dump(i);
}

void BiExpr::dumpInner(const int i) const
{
	Expr::dumpInner(i);
	const std::string opNames[] = {"+", "-", "*", "/", "&&", "||", "==", "!=", "<", "<=", ">", ">="};
	left->dump(i);
	indent(i);
	std::cout << opNames[op] << std::endl;
	right->dump(i);
}

void StringExpr::dumpInner(const int i) const
{
	Expr::dumpInner(i);
	indent(i);
	cout << *str << endl;
}

void IntExpr::dumpInner(const int i) const
{
	Expr::dumpInner(i);
	indent(i);
	cout << num << endl;
}

void FloatExpr::dumpInner(const int i) const
{
	Expr::dumpInner(i);
	indent(i);
	cout << num << endl;
}
void Type::dumpInner(const int i) const
{
	const std::string t[] = {"undef", "int", "float", "void"};
	indent(i);
	std::cout << t[btype] << std::endl;
}

void InitVarDef::dumpInner(const int i) const
{
	name->dump(i);
	expr->dump(i);
}