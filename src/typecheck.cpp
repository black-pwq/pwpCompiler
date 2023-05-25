#include "ast.h"

static bool is_arith_type(BType t)
{
	if (t == BType::bt_int || t == BType::bt_float)
		return true;
	return false;
}

int InitVarDef::typeCheck() const
{
	int errline = expr->typeCheck();
	if (errline)
		return errline;
	VarSymbol *v = static_cast<VarSymbol *>(varSt.lookup(*name->sym));
	VarSymbol *e = static_cast<VarSymbol *>(varSt.lookup(expr->tmpName()));
	assert(v);
	assert(e);
	// match base type

	if (is_arith_type(v->type) && !is_arith_type(e->type))
		return errid = 13, yylineno;
	else if (is_arith_type(v->type))
		return 0;
	else
		assert(false);
	return 0;
}

int IntExpr::typeCheck() const
{
	VarSymbol *n = new SimpleSymbol(BType::bt_int);
	auto r = varSt.insert(tmpName(), n);
	assert(r == n);
	return 0;
}
int FloatExpr::typeCheck() const
{
	VarSymbol *n = new SimpleSymbol(BType::bt_float);
	auto r = varSt.insert(tmpName(), n);
	assert(r == n);
	return 0;
}

int BiExpr::typeCheck() const
{
	int errline;
	if ((errline = left->typeCheck()))
		return errline;
	if ((errline = right->typeCheck()))
		return errline;

	VarSymbol *ls = static_cast<VarSymbol *>(varSt.lookup(left->tmpName()));
	VarSymbol *rs = static_cast<VarSymbol *>(varSt.lookup(right->tmpName()));
	assert(ls);
	assert(rs);
	BType lt = ls->type;
	BType rt = rs->type;
	if (op >= BiOp::bi_add && op <= BiOp::bi_divide)
	{
		if (!is_arith_type(lt) || !is_arith_type(rt))
			return errid = 12, yylineno;
		else if (lt == BType::bt_float || rt == BType::bt_float)
		{
			VarSymbol *n = new SimpleSymbol(BType::bt_float);
			auto r = varSt.insert(tmpName(), n);
			assert(r == n);
			return 0;
		}
		else if (lt == BType::bt_int && rt == BType::bt_int)
		{
			VarSymbol *n = new SimpleSymbol(BType::bt_int);
			auto r = varSt.insert(tmpName(), n);
			assert(r == n);
			return 0;
		}
		else
		{
			assert(false);
			return 0;
		}
	}
	else if (op >= BiOp::bi_eq && op <= BiOp::bi_ge)
	{
		if (is_arith_type(lt) && is_arith_type(rt))
		{
			VarSymbol *n = new SimpleSymbol(BType::bt_bool);
			auto r = varSt.insert(tmpName(), n);
			assert(r == n);
			return 0;
		}
		else
			return errid = 12, yylineno;
	}
	else if (op == BiOp::bi_and || op == BiOp::bi_or)
	{
		if (lt == BType::bt_bool && rt == BType::bt_bool)
		{
			VarSymbol *n = new SimpleSymbol(BType::bt_bool);
			auto r = varSt.insert(tmpName(), n);
			assert(r == n);
			return 0;
		}
		else
			return errid = 11, yylineno;
	}
	else
	{
		assert(false);
	}
	assert(false);
}

int UniExpr::typeCheck() const
{
	int errline = expr->typeCheck();
	if (errline)
		return errline;
	// we assume only basic types are allowed, i.e. compound types like pointers are not in consideration
	VarSymbol *s = static_cast<VarSymbol *>(varSt.lookup(expr->tmpName()));
	if (s->type != BType::bt_bool)
		return errid = 11, yylineno;
	VarSymbol *n = new SimpleSymbol(BType::bt_bool);
	auto r = varSt.insert(tmpName(), n);
	assert(r == s);
	return 0;
}

int Assign::typeCheck() const
{
	int errline = var->typeCheck();
	if (errline)
		return errline;
	return expr->typeCheck();
}

int Return::typeCheck() const
{
	return expr->typeCheck();
}

int For::typeCheck() const
{
	int errline = init->typeCheck();
	if (errline)
		return errline;
	if ((errline = expr->typeCheck()))
		return errline;
	if ((errline = tail->typeCheck()))
		return errline;
	return body->typeCheck();
}
int While::typeCheck() const
{
	int errline = expr->typeCheck();
	if (errline)
		return errline;
	return stmt->typeCheck();
}

int IfElse::typeCheck() const
{
	int errline = expr->typeCheck();
	if (errline != 0)
		return errline;
	if ((errline = ts->typeCheck()))
		return errline;
	return fs->typeCheck();
}

int If::typeCheck() const
{
	int errline = expr->typeCheck();
	if (errline != 0)
		return errline;
	return stmt->typeCheck();
}

int Call::typeCheck() const
{
	FunSymbol *s = static_cast<FunSymbol *>(funSt.lookup(*name));
	// check if the function is defined
	if (s == nullptr)
		return errid = 8, lineno;
	// check if #number of argument match with definition
	if (params->list.size() != s->types.size())
		return errid = 9, lineno;
	for (auto &e : params->list)
	{
		// first check if the symbols used in expr are defined
		int errline = e->typeCheck();
		if (errline)
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
		if ((errline = f->typeCheck()))
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
	if (s == nullptr)
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
		// type info of the variable is inserted to the symbol table,
		// check if the type of init expr matches
		int errline = vardef->typeCheck();
		if (errline)
			return errline;
	}
	return 0;
}