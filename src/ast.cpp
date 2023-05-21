#include "ast.h"
using namespace std;

void BaseAST::dump(const int i) const {
	indent(i);
	std::cout << abi::__cxa_demangle(typeid(*this).name(), NULL, NULL, NULL)
	<< " (#line " << lineno << ") " << "{" << std::endl;
	dumpInner(i + 1);
	indent(i);
	std::cout << "}" << std::endl;
}

void BaseAST::dumpInner(const int i) const {
	std::cout << "NONPRINTABLE" << std::endl;
}

void BaseAST::indent(const int i) const {
	for(int j = 0; j < i; j++)
		std::cout << "  ";
}

llvm::Function *FunDef::codegen()
{

	if(typeid(*name) == typeid(SimpleType)){
		if((static_cast<SimpleType>(*name)).compare("Hello World")){	

		}
	}
    return nullptr;
}

void FunDef::dumpInner(const int i) const
{
    type->dump(i);
	indent(i);
	cout << *name << endl;
	for(auto &f : *fields) 
		f->dump(i);
	block->dump(i);
}

llvm::Value *Return::codegen()
{
    return nullptr;
}

void Return::dumpInner(const int i) const
{
    expr->dump(i);
}

llvm::Value *Block::codegen()
{
    return nullptr;
}

void Block::dumpInner(const int i) const
{
    for(auto &s : stmts) {
		s->dump(i);
	}
}

llvm::Value *CompUnit::codegen()
{
    return nullptr;
}

void CompUnit::dumpInner(const int i) const
{
    for(auto &u : units)
		u->dump(i);
}

llvm::Value *Field::codegen()
{
    return nullptr;
}

void Field::dumpInner(const int i) const
{
    indent(i);
	cout << *type << endl;
	name->dump(i);
}

// llvm::Value *SimpleType::codegen()
// {
	
//     return nullptr;
// }

void SimpleType::dumpInner(const int i) const
{
    indent(i);
	cout << *name << endl;
}

llvm::Value *Expr::codegen()
{
    return nullptr;
}

llvm::Value *UniExpr::codegen()
{
    return nullptr;
}

llvm::Value *BiExpr::codegen()
{
    return nullptr;
}

llvm::Value *ExprList::codegen()
{
    return nullptr;
}

llvm::Value *Assign::codegen()
{
    return nullptr;
}

llvm::Value *If::codegen()
{
    return nullptr;
}

llvm::Value *IfElse::codegen()
{
    return nullptr;
}

llvm::Value *While::codegen()
{
    return nullptr;
}

llvm::Value *For::codegen()
{
    return nullptr;
}

llvm::Value *Var::codegen()
{
    return nullptr;
}

llvm::Value *SimpleVar::codegen()
{
    return nullptr;
}

llvm::Value *ArrayVar::codegen()
{
    return nullptr;
}

llvm::Value *VarDecl::codegen()
{
    return nullptr;
}
