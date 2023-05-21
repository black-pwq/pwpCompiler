#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include "ast.h"

using namespace std;

extern FILE *yyin;
extern int yyparse(unique_ptr<CompUnit> &ast);

int main(int argc, const char *argv[]) {
  auto input = argv[1];

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  assert(yyin);

  unique_ptr<CompUnit> ast;
  auto ret = yyparse(ast);

  assert(!ret);
  ast->dump();
  return 0;
}