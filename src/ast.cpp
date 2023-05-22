#include "ast.h"

using namespace std;


void BaseAST::dump(const int i) const {
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

llvm::Function *FunDef::codegen()
{
	std::vector<Type*> field;
	
	for (int i = 0; i<(*fields).size(); ++i)
	{
 
		if(*((*fields)[i]->type->name)=="int"){
			field.push_back(llvm::Type::getInt32Ty(*TheContext));
		} 
		else if(*((*fields)[i]->type->name)=="float"){
			
		} 
	}
    llvm::Type::getDoubleTy(getGlobalContext()));
	llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(getGlobalContext()),Doubles, false);

  	llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, TheModule.get());
    return nullptr;
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

void CompUnit::dumpInner(const int i) const
{
	for (auto &u : units)
		u->dump(i);
}

void Field::dumpInner(const int i) const
{
	type->dump(i);
	name->dump(i);
}

void Type::dumpInner(const int i) const
{
	indent(i);
	cout << *name << endl;
}

llvm::Value *UniExpr::codegen()
{

	if(op == uni_plus){

	}
	else if(op == uni_minus){
		llvm::Value *res = expr->codegen();
		return res;
	}
	else if(op == uni_not){

	}
	
    return nullptr;
}

llvm::Value *BiExpr::codegen()
{

	llvm::Value *L = left->codegen();
	llvm::Value *R = right->codegen();

	auto typeL = L->getType();
	auto typeR = R->getType();
	if (!L || !R)
		return nullptr;
	// if(typeL == llvm::Type::DoubleTyID)
	switch (op)
	{
		case bi_add:
			return Builder->CreateAdd(L, R, "addtmp");
		case bi_sub:
			return Builder->CreateSub(L, R, "subtmp");
		case bi_times:
			return Builder->CreateMul(L, R, "multmp");
		case bi_divide:
		case bi_lt:
			L = Builder->CreateFCmpULT(L, R, "cmptmp");
	}
    return nullptr;
}
