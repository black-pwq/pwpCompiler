#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include "ast.h"

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
};

int main(int argc, const char *argv[])
{
  auto input = argv[1];

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  assert(yyin);

  unique_ptr<CompUnit> ast;
  auto ret = yyparse(ast);

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
  return 0;
}
