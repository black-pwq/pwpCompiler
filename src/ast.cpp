#include "ast.h"
using namespace std;

int BaseAST::errid;
SymTab BaseAST::varSt;
SymTab BaseAST::funSt;

int Assign::typeCheck() const {
	int errline = var->typeCheck();
	if(errline)
		return errline;
	return expr->typeCheck();
}

int Return::typeCheck() const {
	return expr->typeCheck();
}

int For::typeCheck() const {
	int errline = init->typeCheck();
	if(errline)
		return errline;
	if(( errline = expr->typeCheck() ))
		return errline;
	if(( errline = tail->typeCheck() ))
		return errline;
	return body->typeCheck();
}
int While::typeCheck() const {
	int errline = expr->typeCheck();
	if(errline)
		return errline;
	return stmt->typeCheck();
}

int IfElse::typeCheck() const {
	int errline = expr->typeCheck();
	if(errline != 0)
		return errline;
	if(( errline = ts->typeCheck() ))
		return errline;
	return fs->typeCheck();
}

int If::typeCheck() const {
	int errline = expr->typeCheck();
	if(errline != 0)
		return errline;
	return stmt->typeCheck();
}

int Call::typeCheck() const {
	FunSymbol *s = static_cast<FunSymbol *>(funSt.lookup(*name));
	// check if the function is defined 
	if(s == nullptr)
		return errid = 8, lineno;
	// check if #number of argument match with definition
	if(params->list.size() != s->types.size())
		return errid = 9, lineno;
	for(auto &e : params->list) {
		// first check if the symbols used in expr are defined
		int errline = e->typeCheck();
		if(errline)
			return errline;
		// TODO
		// then check if the types also match
	}
	return 0;
}

int ExprList::typeCheck() const
{
	for (auto &e : list)
	{
		// if e is an Assign
		int errline = e->typeCheck();
		if (errline != 0)
			return errline;
	}
	return 0;
}
int Block::typeCheck() const
{
	varSt.openScope();
	for (auto &i : items)
	{
		int errline = i->typeCheck();
		if (errline != 0)
			return errline;
	}
	varSt.closeScope();
	return 0;
}

int FunDef::typeCheck() const
{
	int errline;
	varSt.openScope();
	for (auto &f : *fields)
	{
		if (( errline = f->typeCheck() ))
			return errline;
	}
	FunSymbol *s = new FunSymbol(type->btype);
	for (auto &f : *fields)
	{
		auto &key = f->name->sym;
		// we must find the coresponding symbols, otherwise this function returns with error
		s->appendType(static_cast<VarSymbol *>(varSt.lookup(*key)));
	}
	Symbol *r = funSt.insert(*name, s);
	if (r != s)
		return errid = 7, lineno;
	errline = block->typeCheck();
	varSt.closeScope();
	return errline;
}

int Field::typeCheck() const
{
	auto &exprs = name->exprs->list;
	VarSymbol *s = nullptr;
	if (exprs.size() == 0)
		s = new SimpleSymbol(type->btype);
	else
	{
		s = new ArraySymbol(type->btype);
		for (auto &e : exprs)
		{
			if (!e->evaluable)
				return errid = 1, lineno;
			else if (e->num <= 0)
				return errid = 2, lineno;
			else
				s->append(e->num);
		}
	}
	assert(s != nullptr);
	auto r = varSt.insert(*name->sym, s);
	if (r != s)
		return errid = 3, lineno;
	return 0;
}

int CompUnit::typeCheck() const
{
	for (auto &unit : units)
	{
		int errline = unit->typeCheck();
		if (errline != 0)
			return errline;
	}
	Symbol *s = funSt.lookup("main");
	if(s == nullptr)
		return errid = 10, lineno;
	return 0;
}

int SimpleVar::typeCheck() const
{
	VarSymbol *s = static_cast<VarSymbol *>(varSt.lookup(*sym));
	if (s == nullptr)
		return errid = 4, lineno;
	else if (s->wordsInDim.size() != 0)
		return errid = 5, lineno;
	return 0;
}

int ArrayVar::typeCheck() const
{
	VarSymbol *s = static_cast<VarSymbol *>(varSt.lookup(*sym));
	if (s == nullptr)
		return errid = 4, lineno;
	else if (s->wordsInDim.size() != exprs->list.size())
		return errid = 6, lineno;
	return 0;
}

int VarDecl::typeCheck() const
{
	for (auto &vardef : *vars)
	{
		auto &name = vardef->name;
		auto &exprs = name->exprs->list;
		VarSymbol *s = nullptr;
		if (exprs.size() == 0)
			s = new SimpleSymbol(type->btype);
		else
		{
			s = new ArraySymbol(type->btype);
			for (auto &e : exprs)
			{
				if (!e->evaluable)
					return errid = 1, Unit::lineno;
				else if (e->num <= 0)
					return errid = 2, Unit::lineno;
				else
					s->append(e->num);
			}
		}
		assert(s != nullptr);
		auto r = varSt.insert(*name->sym, s);
		if (r != s)
			return errid = 3, Unit::lineno;
	}
	return 0;
}

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
	std::cout << t[btype];
	for (int i = 0; i < dim; i++)
		std::cout << "[]";
	std::cout << std::endl;
}
