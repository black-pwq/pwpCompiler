#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;
static std::map<std::string, llvm::Value *> NameValues;

int main(){
 TheModule = std::make_unique<llvm::Module>("hello,llvm",TheContext);

 TheModule -> dump();
 return 0;
}
