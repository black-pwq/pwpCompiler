#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include "ast.h"

using namespace std;

extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);

int main(int argc, const char *argv[]) {
  // compiler 模式 输入文件 -o 输出文件
  assert(argc == 5);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  assert(yyin);

  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  assert(!ret);

  ast->dump();
  return 0;
}
