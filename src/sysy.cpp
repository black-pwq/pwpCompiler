#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include "ast.h"
#include "llvm.h"

using namespace std;

extern FILE *yyin;
extern int errid;
extern SymTab varSt;
extern SymTab funSt;
extern int yyparse(unique_ptr<CompUnit> &ast);

std::string errtab[] = {
    "PASS",
    "array size is not computable at compile time",
    "array size is non-poisitive",
    "already declared variable in the same scope",
    "variable is not defined so far",
    "variable (compound) type mismatch with definition",
    "array dimention mismath with definition",
    "fun redefinition",
    "fun definition not find",
    "#arguments mismatch with the definition of function",
    "no main fucntion",
    "expression type mismatch (expected boolean)",
    "expression type mismatch (expected arithmetic)",
    "(base) type conversion fails",
    "(compound) type conversion fails",
    "pointer biop pointer is not allowed",
    "array access out of dimension",
    "parameter (base) type mismatch",
    "parameter (compound) type mismatch",
    "binary operations on offset of pointers are not allowed except add/sub",
    "break outside loop",
    "continue outside loop",
};

int main(int argc, const char *argv[])
{
  auto input = argv[1];

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  // FILE *yyout = fopen("out.ll","w");
  assert(yyin);
  unique_ptr<CompUnit> ast;
  auto ret = yyparse(ast);

  
  auto s1 = new FunSymbol(bt_void);
  auto s2 = new FunSymbol(bt_void);

  s1->appendType(new SimpleSymbol(bt_int));
  s2->appendType(new SimpleSymbol(bt_float));

  funSt.insert("getint",new FunSymbol(bt_int));
  funSt.insert("getfloat",new FunSymbol(bt_float));

  funSt.insert("putint",s1);
  funSt.insert("putfloat",s2);

  assert(!ret);
  ast->dump();
  int res = ast->typeCheck();
  auto varout = fopen("var.dump", "w");
  auto funout = fopen("fun.dump", "w");
  
  varSt.dump(varout);
  funSt.dump(funout);
  if (res)
    cout << "err lineno = " << res << endl;
  cout << errtab[errid] << endl;


  initmodule();
  initiostream();

  ast->unitcodegen();

  
  TheModule->print(llvm::outs(), nullptr);  
  auto TargetTriple = llvm::sys::getProcessTriple();
  TargetTriple = "riscv64-unknown-elf";
  std::cout << TargetTriple << std::endl;

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();


  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
  if (!Target) {
    llvm::errs() << Error;
    return 1;
  }
  auto CPU = "generic-rv64";
  auto Features = "";

  llvm::TargetOptions opt;
  auto RM = llvm::Optional<llvm::Reloc::Model>();
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
  // auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
  auto Filename = "output.s";
  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }
  llvm::legacy::PassManager pass;
  auto FileType = llvm::CGFT_AssemblyFile;

  if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    llvm::errs() << "TargetMachine can't emit a file of this type";
    return 1;
  }
  pass.run(*TheModule);
  dest.flush();


  return 0;
}