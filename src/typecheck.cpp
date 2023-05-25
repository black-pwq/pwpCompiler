#include "ast.h"
using namespace std;

int errid;
SymTab varSt;
SymTab funSt;

static bool is_arith_type(BType t)
{
	if (t == BType::bt_int || t == BType::bt_float)
		return true;
	return false;
}
static int check_var_expr_base_type(const VarSymbol *vs, const VarSymbol *es)
{
	// mix of int and float is allowed
	if (is_arith_type(vs->type) && is_arith_type(es->type))
		return 0;
	// branch for other types
	return errid = 13;
}
static int check_var_expr_type(const VarSymbol *vs, const VarSymbol *es)
{
	assert(vs);
	assert(es);
	if ((errid = check_var_expr_base_type(vs, es)))
		return errid;
	else if (vs->wordsInDim.size() != es->wordsInDim.size())
		return errid = 14;
	return 0;
}

/**
 * Type info already inseted into symbol table by VarDecl,
 * check if the type of the expression matches the definition.
 */
int InitVarDef::typeCheck() const
{
	VarSymbol *v = static_cast<VarSymbol *>(varSt.lookup(*var->nameSym->name));
	int errline = expr->typeCheck();
	if (errline)
		return errline;
	VarSymbol *e = static_cast<VarSymbol *>(varSt.lookup(expr->tmpName()));

	if ((errid = check_var_expr_type(v, e)))
		return lineno;
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
	// both base type
	if (ls->wordsInDim.size() == 0 && rs->wordsInDim.size() == 0)
	{
		BType lt = ls->type;
		BType rt = rs->type;
		if (op >= BiOp::bi_add && op <= BiOp::bi_divide)
		{
			// neither of them is int or float (arith type)
			if (!is_arith_type(lt) || !is_arith_type(rt))
				return errid = 12, left->lineno;
			// one of them is float
			else if (lt == BType::bt_float || rt == BType::bt_float)
			{
				VarSymbol *n = new SimpleSymbol(BType::bt_float);
				assert(varSt.insert(tmpName(), n) == n); // == n
				return 0;
			}
			// both are int
			else if (lt == BType::bt_int && rt == BType::bt_int)
			{
				VarSymbol *n = new SimpleSymbol(BType::bt_int);
				assert(varSt.insert(tmpName(), n) == n); // == n
				return 0;
			}
			else
				assert(false);
		}
		// comprasion
		else if (op >= BiOp::bi_eq && op <= BiOp::bi_ge)
		{
			if (is_arith_type(lt) && is_arith_type(rt))
			{
				VarSymbol *n = new SimpleSymbol(BType::bt_bool);
				assert(varSt.insert(tmpName(), n) == n); // == n
				return 0;
			}
			else
				return errid = 12, left->lineno;
		}
		// logic
		else if (op == BiOp::bi_and || op == BiOp::bi_or)
		{
			if (lt == BType::bt_bool && rt == BType::bt_bool)
			{
				VarSymbol *n = new SimpleSymbol(BType::bt_bool);
				assert(varSt.insert(tmpName(), n) == n); // == n
				return 0;
			}
			else
				return errid = 11, left->lineno;
		}
		else
			assert(false);
	}
	// both are compound type
	else if (ls->wordsInDim.size() != 0 && rs->wordsInDim.size() != 0)
		return errid = 15, left->lineno;
	// poiner add/sub offset
	else
	{
		if (op != BiOp::bi_add && op != BiOp::bi_sub)
			return errid = 19, left->lineno;
		auto s = ls->wordsInDim.size() != 0 ? ls : rs;
		VarSymbol *n = new ArraySymbol(static_cast<ArraySymbol *>(s));
		assert(varSt.insert(tmpName(), n) == n); // == n
		return 0;
	}
	// returned in each block
	assert(false);
}

int UniExpr::typeCheck() const
{
	int errline = expr->typeCheck();
	if (errline)
		return errline;
	// we assume only basic types are allowed, i.e. compound types like pointers are not in consideration
	VarSymbol *s = static_cast<VarSymbol *>(varSt.lookup(expr->tmpName()));
	if (op == UniOp::uni_not)
		if (s->type != BType::bt_bool)
			return errid = 11, expr->lineno;
		else
			return 0;
	// op is - or +
	else if (!is_arith_type(s->type))
		return errid = 13, expr->lineno;
	// fall to return
	VarSymbol *n = new SimpleSymbol(BType::bt_bool);
	auto r = varSt.insert(tmpName(), n);
	assert(r == n);
	return 0;
}

int Assign::typeCheck() const
{
	int errline;
	if ((errline = var->typeCheck()))
		return errline;
	if ((errline = expr->typeCheck()))
		return errline;
	auto vs = static_cast<VarSymbol *>(varSt.lookup(var->tmpName()));
	auto es = static_cast<VarSymbol *>(varSt.lookup(expr->tmpName()));
	if ((errid = check_var_expr_type(vs, es)))
		return lineno;
	if (var->nameSym->symbol->wordsInDim.size() == 0)
		varSt.insert(tmpName(), new SimpleSymbol(var->nameSym->symbol));
	else
		varSt.insert(tmpName(), new ArraySymbol(var->nameSym->symbol));
	return 0;
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
	// predifined vararg funcion
	if (*name == "printf" || *name == "scanf")
		return 0;
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
	}
	// then check if the types also match
	auto &exprs = params->list;
	for (std::vector<std::unique_ptr<Expr>>::size_type i = 0; i < params->list.size(); ++i)
	{
		auto es = static_cast<VarSymbol *>(varSt.lookup(exprs[i]->tmpName()));
		assert(es);
		if (es->type != s->types[i]->type)
			return errid = 17, exprs[i]->lineno;
		else if (es->wordsInDim.size() != s->types[i]->wordsInDim.size())
		{
			cout << es->wordsInDim.size() << endl;
			cout << s->types[i]->wordsInDim.size() << endl;
			return errid = 18, exprs[i]->lineno;
		}
		// else ok
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

	FunSymbol *s = new FunSymbol(type->btype);
	for (auto &f : *fields)
	{
		if ((errline = f->typeCheck()))
			return errline;
		auto r = static_cast<VarSymbol *>(varSt.lookup(*f->var->nameSym->name));
		// we must find the coresponding symbols, otherwise this function returns with error
		assert(r);
		s->appendType((r));
	}
	Symbol *r = funSt.insert(*nameSym->name, s);
	if (r != s)
		return errid = 7, lineno;
	nameSym->symbol = s;
	errline = block->typeCheck();
	varSt.closeScope();
	return errline;
}

int Field::typeCheck() const
{
	auto &exprs = var->exprs->list;
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
	var->nameSym->symbol = s;
	auto r = varSt.insert(*var->nameSym->name, s);
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

/**
 * SimpleVar could be an array identifier, or simplely a var declared with a base type
 */
int SimpleVar::typeCheck() const
{
	// look for declaration
	VarSymbol *s = static_cast<VarSymbol *>(varSt.lookup(*nameSym->name));
	if (s == nullptr)
		return errid = 4, lineno;
	// sub-array add/sub
	else if (s->wordsInDim.size() != 0)
	{
		VarSymbol *n = new ArraySymbol(static_cast<ArraySymbol *>(s));
		varSt.insert(tmpName(), n);
		// assert(varSt.insert(tmpName(), n) == n);
		nameSym->symbol = n;
		// fall to return
	}
	// simple variable use
	else
	{
		auto n = new SimpleSymbol(s->type);
		varSt.insert(tmpName(), n);
		// assert(varSt.insert(tmpName(), n) == n);
		nameSym->symbol = n;
		// fall to return
	}
	return 0;
	/**
	 * basically speaking we shall assert(r == n) in the above 2 blocks, however,
	 * the assert will fails if we have the binary assignment like a += 2.
	 */
}

/**
 * ArrayVar could be used for
 * 1. array elements access, e.g. a[2][3];
 * 2. sub-array add/sub operations, e.g. a[2] + 1;
 * where a is a 2-dim array.
 *
 * Array declaration is considered in VarDecl.
 */
int ArrayVar::typeCheck() const
{
	// look the declareation
	VarSymbol *s = static_cast<VarSymbol *>(varSt.lookup(*nameSym->name));
	if (s == nullptr)
		return errid = 4, lineno;
	// out of dimension
	else if (s->wordsInDim.size() < exprs->list.size())
		return errid = 16, lineno;
	// element access
	else if (s->wordsInDim.size() == exprs->list.size())
	{
		VarSymbol *n = new SimpleSymbol(s->type);
		varSt.insert(tmpName(), n);
		// assert(varSt.insert(tmpName(), n) == n); // == n
		nameSym->symbol = n;
		// fall to return
	}
	// add/sub
	else
	{
		VarSymbol *n = new ArraySymbol(s->type);
		for (auto i = exprs->list.size(); i < s->wordsInDim.size(); i++)
			n->append(s->wordsInDim[i]);
		varSt.insert(tmpName(), n);
		// assert(varSt.insert(tmpName(), n) == n); // == n
		nameSym->symbol = n;
		// fall to return
	}
	return 0;
	/**
	 * Basically speaking we shall assert(r == n) in the above 2 blocks, however,
	 * the assert will fails if we have the binary assignment like a += 2.
	 * This situation will only appeared with Var (i.e., SimpleVar and ArrayVar)
	 */
}

int VarDecl::typeCheck() const
{
	for (auto &vardef : *vardefs)
	{
		auto &var = vardef->var;
		auto &exprs = var->exprs->list;
		VarSymbol *s = nullptr;
		// simple variables
		if (exprs.size() == 0)
			s = new SimpleSymbol(type->btype);
		// compound type liek array
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
		// insert the declared symbol into symbol table
		assert(s != nullptr);
		auto r = varSt.insert(*var->nameSym->name, s);
		if (r != s)
			return errid = 3, Unit::lineno;
		// type info of the variable is inserted to the symbol table,
		// check if the type of init expr matches
		int errline = vardef->typeCheck();
		if (errline)
			return errline;
		var->nameSym->symbol = s;
	}
	return 0;
}