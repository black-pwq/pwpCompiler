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
  
}
