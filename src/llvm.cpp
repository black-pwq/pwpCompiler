#include "llvm.h"

 std::unique_ptr<llvm::LLVMContext> TheContext;
 std::unique_ptr<llvm::Module> TheModule;
std::unique_ptr<llvm::IRBuilder<>> Builder;

void initmodule()
{
  // Open a new context and module.
  TheContext = std::make_unique<llvm::LLVMContext>();
  TheModule = std::make_unique<llvm::Module>("my module", *TheContext);

  // Create a new builder for the module.

  Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
  std::cout<<"init fin________"<<std::endl;
}

void initiostream()
{

    initgetfloat();
    initgetint();
    initputfloat();
    initputint();
}


void initgetfloat(){

}

void initgetint(){

std::vector<llvm::Type*> field;
    // field.push_back(llvm::Type::getVoidTy(*TheContext));
	  auto ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext),field,false);
  	llvm::Function *F = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "getint", TheModule.get());
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
  	Builder->SetInsertPoint(BB);

    // llvm::AllocaInst *allocDeclrInt = Builder->CreateAlloca(llvm::IntegerType::getInt32Ty(*TheContext), NULL, "a");

    // llvm::Value *itmp = Builder->CreateLoad(llvm::IntegerType::getInt32Ty(*TheContext),F->getArg(0),"itmp");

    // find the function printf
    std::vector<llvm::Type *> inArgs;
    inArgs.push_back(Builder->getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef2(inArgs);
	//scanf def
    llvm::FunctionType *inType =llvm::FunctionType::get(Builder->getInt32Ty(), argsRef2, true);
    llvm::Function *inFunc = llvm::dyn_cast<llvm::Function>(TheModule->getOrInsertFunction("scanf", inType).getCallee());
    
    llvm::AllocaInst *allocDeclrInt = Builder->CreateAlloca(llvm::IntegerType::getInt32Ty(*TheContext), NULL, "tmp_add");

    //setting the field of the printf
    std::vector<llvm::Value*> field4;
	  field4.push_back(Builder->CreateGlobalStringPtr("%d"));
	  field4.push_back(allocDeclrInt);
    Builder->CreateCall(inFunc,field4);
	  
	
    Builder->CreateRet(Builder->CreateLoad(llvm::IntegerType::getInt32Ty(*TheContext),allocDeclrInt));

}

void initputint(){

    std::vector<llvm::Type*> field;
    field.push_back(llvm::Type::getInt32Ty(*TheContext));
	  auto ft = llvm::FunctionType::get(llvm::Type::getVoidTy(*TheContext),field,false);
  	llvm::Function *F = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "putint", TheModule.get());
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
  	Builder->SetInsertPoint(BB);

    // llvm::AllocaInst *allocDeclrInt = Builder->CreateAlloca(llvm::IntegerType::getInt32Ty(*TheContext), NULL, "a");

    // llvm::Value *itmp = Builder->CreateLoad(llvm::IntegerType::getInt32Ty(*TheContext),F->getArg(0),"itmp");

    // find the function printf
    std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(Builder->getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
    llvm::FunctionType *putsType =llvm::FunctionType::get(Builder->getInt32Ty(), argsRef, true);
    llvm::Function *putsFunc = llvm::dyn_cast<llvm::Function>(TheModule->getOrInsertFunction("printf", putsType).getCallee());
    
    //setting the field of the printf
    std::vector<llvm::Value*> field4;
	  field4.push_back(Builder->CreateGlobalStringPtr("%d\n"));
	  field4.push_back(F->getArg(0));
    Builder->CreateCall(putsFunc,field4);
	  
	
    Builder->CreateRetVoid();
}

void initputfloat(){
    //def of putfloat

	  
}