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

void FunDef::codegen()
{

	
	std::vector<llvm::Type*> field;
	std::cout<<(*fields).size()<<std::endl;
	for (int i = 0; i<(*fields).size(); ++i)
	{
 
		if(*((*fields)[i]->type->name)=="int"){
			field.push_back(llvm::Type::getInt32Ty(*TheContext));
		} 
		else if(*((*fields)[i]->type->name)=="float"){
			field.push_back(llvm::Type::getFloatTy(*TheContext));
		} 

	}
	llvm::FunctionType *ft;
	if(*(type->name) == "int"){
		std::cout<<"int func" <<std::endl;
		ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext),field,false);
	}
	else if(*(type->name) == "float"){
		ft = llvm::FunctionType::get(llvm::Type::getFloatTy(*TheContext),field,false);

	}
	else if(*(type->name) == "string"){
		// ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext),field,false);

	}else if(*(type->name) == "void"){
		ft = llvm::FunctionType::get(llvm::Type::getVoidTy(*TheContext),field,false);
	}
	else{
		std::cout<<"fundef fault"<<std::endl;
	}


	// llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(getGlobalContext()),Doubles, false);
  	llvm::Function *F = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, *name, TheModule.get());
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
  	Builder->SetInsertPoint(BB);
	// llvm::ReturnInst::Create(*TheContext, llvm::ConstantInt::get(*TheContext,llvm::APSInt(32,false)), BB);
	// Builder->CreateRetVoid();
    // Builder->CreateRet();
	// Builder->CreateRet(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*TheContext),0));

	//fff
	llvm::AllocaInst *allocDeclrInt = Builder->CreateAlloca(llvm::IntegerType::get(TheModule->getContext(), 32), NULL, "a.addr");
    allocDeclrInt->setAlignment(llvm::Align(4));



	std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(Builder->getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

    llvm::FunctionType *putsType =llvm::FunctionType::get(Builder->getInt32Ty(), argsRef, true);
    llvm::Function *putsFunc = llvm::dyn_cast<llvm::Function>(TheModule->getOrInsertFunction("printf", putsType).getCallee());


	std::vector<llvm::Type *> inArgs;
    inArgs.push_back(Builder->getInt8Ty()->getPointerTo());
	
    llvm::ArrayRef<llvm::Type*>  argsRef2(inArgs);

    llvm::FunctionType *inType =llvm::FunctionType::get(Builder->getInt32Ty(), argsRef2, true);
    llvm::Function *inFunc = llvm::dyn_cast<llvm::Function>(TheModule->getOrInsertFunction("scanf", inType).getCallee());
	
	std::vector<llvm::Value*> field1;
	field1.push_back(Builder->CreateGlobalStringPtr("%d"));
	field1.push_back(allocDeclrInt);
    Builder->CreateCall(inFunc,field1);


    //输出换行
	std::vector<llvm::Value*> field2;
	field2.push_back(Builder->CreateGlobalStringPtr("\n%f\n%d"));
	field2.push_back(llvm::ConstantFP::get(llvm::Type::getFloatTy(*TheContext), llvm::APFloat(9.00)));



	field2.push_back(Builder->CreateLoad(llvm::IntegerType::get(TheModule->getContext(), 32),allocDeclrInt, "a.addr"));

	// new llvm::LoadInst(, "", false, BB);
		
    Builder->CreateCall(putsFunc,field2);

	Builder->CreateRet(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*TheContext),0));
	//ffff

    verifyFunction(*F);

	// return F;
}

void Unit::codegen()
{
	std::cout<<"Unit init" <<std::endl;
    // return nullptr;
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


void CompUnit::codegen()
{
	// initmodule();
	std::cout<<"compunit count " << units.size() <<std::endl;
	for (int i = 0; i<units.size(); i++)
	{
 
		units[i]->codegen();
	}
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


llvm::Value *Call::codegen()
{
    return nullptr;
}

llvm::Value *Expr::codegen()
{

    return nullptr;
}

llvm::Value *ExprList::codegen()
{
    return nullptr;
}
