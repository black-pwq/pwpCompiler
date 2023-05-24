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

	//io
	llvm::AllocaInst *allocDeclrInt = Builder->CreateAlloca(llvm::IntegerType::getInt32Ty(*TheContext), NULL, "a.addr");
    allocDeclrInt->setAlignment(llvm::Align(4));

	std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(Builder->getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

    llvm::FunctionType *putsType =llvm::FunctionType::get(Builder->getInt32Ty(), argsRef, true);
    llvm::Function *putsFunc = llvm::dyn_cast<llvm::Function>(TheModule->getOrInsertFunction("printf", putsType).getCallee());


	std::vector<llvm::Type *> inArgs;
    inArgs.push_back(Builder->getInt8Ty()->getPointerTo());
	
    llvm::ArrayRef<llvm::Type*>  argsRef2(inArgs);
	//scanf def
    llvm::FunctionType *inType =llvm::FunctionType::get(Builder->getInt32Ty(), argsRef2, true);
    llvm::Function *inFunc = llvm::dyn_cast<llvm::Function>(TheModule->getOrInsertFunction("scanf", inType).getCallee());
	
	std::vector<llvm::Value*> field1;
	field1.push_back(Builder->CreateGlobalStringPtr("%d"));
	field1.push_back(allocDeclrInt);
    Builder->CreateCall(inFunc,field1);


	std::vector<llvm::Value*> field2;

	field2.push_back(Builder->CreateGlobalStringPtr("\n%d"));
	// field2.push_back(llvm::ConstantFP::get(llvm::Type::getFloatTy(*TheContext), llvm::APFloat(9.00)));
	field2.push_back(Builder->CreateLoad(llvm::IntegerType::get(TheModule->getContext(), 32),allocDeclrInt, "a.addr"));

	// new llvm::LoadInst(, "", false, BB);
		
    Builder->CreateCall(putsFunc,field2);

	//io


	//arraytest

    // Builder->CreateStore(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*TheContext),0), allocDeclrInt);

	// llvm::Value *tem = Builder->CreateLoad(llvm::IntegerType::get(TheModule->getContext(), 32),allocDeclrInt, "a.addr");
	
	llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt32Ty(*TheContext),100);
	llvm::Value* array = Builder->CreateAlloca(arrayType );

	// Builder->CreateStore(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0), Builder->CreateConstGEP(array,4));
	llvm::Value* i_allo = Builder->CreateAlloca(llvm::Type::getInt32Ty(*TheContext));
	Builder->CreateStore(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0), i_allo);
	//for 



	llvm::BasicBlock* for_cont = llvm::BasicBlock::Create(*TheContext, "for.cond", F);
	llvm::BasicBlock* for_body = llvm::BasicBlock::Create(*TheContext, "for.body", F);
	llvm::BasicBlock* for_end = llvm::BasicBlock::Create(*TheContext, "for.end", F);
	Builder->CreateBr(for_cont);
	
	//for_cont基本块
	// Builder->SetInsertPoint(for_cont);
	std::cout<<allocDeclrInt->getType() << "  " << llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 5)->getType()<< std::endl;
	// std::cout<<(allocDeclrInt->getType()) << "  " << (llvm::cast<llvm::IntegerType>(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 4))).getType()<< std::endl;

	// llvm::Value* i_load = Builder->CreateLoad(llvm::IntegerType::get(TheModule->getContext(), 32),allocDeclrInt);
	llvm::Value* i_load = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0);
	llvm::Value* icmp = Builder->CreateICmpSLT(icmp, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 5));
	Builder->CreateCondBr(icmp, for_body, for_end);
	
	//for_body基本块
	Builder->SetInsertPoint(for_body);
	std::vector<llvm::Value*> Idxs;
	Idxs.push_back(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0));
	Idxs.push_back(i_load);

	llvm::Value* array_i = Builder->CreateGEP(arrayType,array, Idxs);//使用gep指令获取元素地址的指令的方式有好几个，最好都掌握
	
	//get each i using io

	std::vector<llvm::Value*> field3;
	field3.push_back(Builder->CreateGlobalStringPtr("%d"));
	field3.push_back(i_allo);
    Builder->CreateCall(inFunc,field3);

	std::vector<llvm::Value*> field4;
	field4.push_back(Builder->CreateGlobalStringPtr("\n%d"));
	field4.push_back(Builder->CreateLoad(llvm::IntegerType::get(TheModule->getContext(), 32),i_allo, "a.addr"));
    Builder->CreateCall(putsFunc,field4);
	//get each i using io


	llvm::Value* i_add = Builder->CreateAdd(i_load, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 1));
	Builder->CreateStore(i_add, i_allo);
	Builder->CreateBr(for_cont);
	
	//for_end基本块，注意这里的返回值不能直接使用for_body中的sum，而是要用load指令再取一次
	Builder->SetInsertPoint(for_end);
	//for 

	//arraytest
	Builder->CreateRet(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*TheContext),0));



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
